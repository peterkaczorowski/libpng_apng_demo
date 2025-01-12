#include <wx/wx.h>
#include <png.h>
#include <vector>
#include <stdexcept>
#include <iostream>

// Application class
class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

// Main window class
class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title, const wxImage& image1, const wxImage& image2);

private:
    wxImage m_image1;
    wxImage m_image2;
    void OnPaint(wxPaintEvent& event);

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_PAINT(MyFrame::OnPaint)
wxEND_EVENT_TABLE()

// Helper function to load APNG frames using libpng
std::vector<wxImage> LoadAPNG(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) throw std::runtime_error("Cannot open PNG file!");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) throw std::runtime_error("Failed to create PNG structure!");

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, nullptr, nullptr);
        throw std::runtime_error("Failed to create info structure!");
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, nullptr);
        fclose(fp);
        throw std::runtime_error("Error processing PNG file!");
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    // Display PNG file info
    std::cout << "PNG File Info:" << std::endl;
    std::cout << "  Width: " << width << " px" << std::endl;
    std::cout << "  Height: " << height << " px" << std::endl;
    std::cout << "  Bit Depth: " << static_cast<int>(bit_depth) << " bit" << std::endl;

    std::string color_type_str;
    switch (color_type) {
        case PNG_COLOR_TYPE_GRAY: color_type_str = "Gray"; break;
        case PNG_COLOR_TYPE_PALETTE: color_type_str = "Palette"; break;
        case PNG_COLOR_TYPE_RGB: color_type_str = "RGB"; break;
        case PNG_COLOR_TYPE_RGB_ALPHA: color_type_str = "RGBA"; break;
        case PNG_COLOR_TYPE_GRAY_ALPHA: color_type_str = "Gray+Alpha"; break;
        default: color_type_str = "Unknown"; break;
    }
    std::cout << "  Color Type: " << color_type_str << std::endl;

    if (bit_depth == 16) png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER); // Add full transparency

    png_set_gray_to_rgb(png); // Convert grayscale images to RGB
    png_read_update_info(png, info);

    // Load APNG frames
    std::vector<wxImage> frames;
    int frame_index = 0;
    while (frame_index < 2) { // Limit to a maximum of two frames
        std::cout << "Loading frame " << frame_index << ":" << std::endl;

        std::vector<png_bytep> row_pointers(height);
        std::vector<png_byte> data(width * height * 4); // RGBA
        for (int y = 0; y < height; ++y) {
            row_pointers[y] = data.data() + y * width * 4;
        }

        png_read_image(png, row_pointers.data());

        // Display frame data
        std::cout << "  Frame " << frame_index << " width: " << width << " px, height: " << height << " px" << std::endl;

        // Convert data to wxImage
        wxImage image(width, height);
        image.InitAlpha(); // Handle transparency

        for (int y = 0; y < height; ++y) {
            unsigned char* dst_rgb = image.GetData() + y * width * 3; // RGB
            unsigned char* dst_alpha = image.GetAlpha() + y * width;  // Alpha
            unsigned char* src = data.data() + y * width * 4;         // RGBA
            for (int x = 0; x < width; ++x) {
                dst_rgb[0] = src[0];    // R
                dst_rgb[1] = src[1];    // G
                dst_rgb[2] = src[2];    // B
                dst_alpha[0] = src[3];  // A
                dst_rgb += 3;
                dst_alpha++;
                src += 4;
            }
        }

        frames.push_back(image);
        frame_index++;

        // Check if there are more frames
        if (setjmp(png_jmpbuf(png))) {
            break; // Exit if no more frames
        }

        try {
            png_read_frame_head(png, info); // Load next frame header
        } catch (...) {
            std::cerr << "Error reading frame header." << std::endl;
            break;
        }
    }

    png_destroy_read_struct(&png, &info, nullptr);
    fclose(fp);

    return frames;
}

bool MyApp::OnInit() {
    try {
        // Path to the PNG file
        const char* filename = "example.apng";

        // Load APNG frames using libpng
        std::vector<wxImage> frames = LoadAPNG(filename);
        if (frames.size() < 2) throw std::runtime_error("APNG file does not contain enough frames!");

        // Create the application window
        MyFrame* frame = new MyFrame("APNG Viewer", frames[0], frames[1]);
        frame->Show(true);
        return true;
    } catch (const std::exception& e) {
        wxLogError("Error: %s", e.what());
        return false;
    }
}

MyFrame::MyFrame(const wxString& title, const wxImage& image1, const wxImage& image2)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(image1.GetWidth() + image2.GetWidth(), std::max(image1.GetHeight(), image2.GetHeight()))),
      m_image1(image1), m_image2(image2) {}

void MyFrame::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    wxBitmap bitmap1(m_image1);
    wxBitmap bitmap2(m_image2);
    dc.DrawBitmap(bitmap1, 0, 0, false);
    dc.DrawBitmap(bitmap2, m_image1.GetWidth(), 0, false);
}

wxIMPLEMENT_APP(MyApp);


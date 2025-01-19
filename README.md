# libpng_apng_read_demo

This repository contains a demonstration program that uses the **libpng** library to read and process APNG (Animated PNG) files. The program loads frames from an APNG file and displays them side-by-side in a graphical window using the **wxWidgets** framework.

## Features
- Reads APNG files using **libpng** modified with APNG support patch.
- Supports transparency and converts grayscale images to RGB.
- Displays the first two frames of an APNG file in a simple graphical interface.
- Provides detailed output about the PNG file's properties, such as width, height, bit depth, and color type.
- Includes error handling for common issues during file reading.

## Requirements
- **libpng**: For processing PNG and APNG files.
- **wxWidgets**: For creating the graphical user interface.
- **CMake**: For building the project.
- **C++17** or newer.

## How to Build

1. Clone the repository:
   ```bash
   git clone https://github.com/peterkaczorowski/libpng_apng_demo.git
   cd libpng_apng_demo
   ```

2. Install dependencies:
   - Ensure you have **libpng**, **wxWidgets**, and **CMake** installed on your system.

3. Build the program using CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Run the program:
   ```bash
   ./PNGViewer
   ```

## File Structure
- `main.cpp`: The core implementation of the program.
- `CMakeLists.txt`: Configuration file for building the project with CMake.
- `README.md`: This description file.
- Additional assets like `example.apng` (optional for demonstration).

## Usage
- Replace `example.apng` with the path to your APNG file.
- The program will display the first two frames of the file in a window.

## Dependencies
- Patched **libpng** from: https://github.com/peterkaczorowski/libpng/tree/v1.6.44-apng

## License
This project is released under the [MIT License](LICENSE). You are free to use, modify, and distribute the code.



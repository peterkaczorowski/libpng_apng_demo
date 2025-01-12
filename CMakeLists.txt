cmake_minimum_required(VERSION 3.15)
project(PNGViewer)

set(CMAKE_CXX_STANDARD 17)

# Znajdź wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)

# Znajdź libpng
find_package(PNG REQUIRED)

# Dodaj plik źródłowy
add_executable(PNGViewer main.cpp)

# Połącz z bibliotekami wxWidgets i libpng
target_link_libraries(PNGViewer PRIVATE ${wxWidgets_LIBRARIES} PNG::PNG)
target_include_directories(PNGViewer PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_compile_definitions(PNGViewer PRIVATE ${wxWidgets_DEFINITIONS})


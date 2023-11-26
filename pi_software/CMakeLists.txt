cmake_minimum_required(VERSION 3.0)
project(laserTracker)

# Find packages
find_package(OpenCV REQUIRED)
find_package(simpleble REQUIRED CONFIG)

# Specify the include directory
include_directories(include)

# Add the executable
add_executable(test src/test.cpp src/background.cpp src/util.cpp src/myble.cpp)

# Specify include directories
target_include_directories(test PRIVATE
    /usr/local/include/libcamera-apps/
    /usr/include/opencv4/
    /usr/local/include/libcamera
)

# Specify library directories
link_directories(/usr/local/lib/aarch64-linux-gnu/)

# Link with the libraries
target_link_libraries(test PRIVATE camera_app camera ${OpenCV_LIBS} simpleble::simpleble)

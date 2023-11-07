cmake_minimum_required(VERSION 3.0)
project(YourProjectName)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Add the executable
add_executable(test test.cpp background.cpp util.cpp)

# Specify include directories
target_include_directories(test PRIVATE
    /usr/local/include/libcamera-apps/
    /usr/include/opencv4/
    /usr/local/include/libcamera
)

# Specify library directories
link_directories(/usr/local/lib/aarch64-linux-gnu/)

# Link with the camera_app library
target_link_libraries(test PRIVATE camera_app)
target_link_libraries(test PRIVATE camera)
# Link with the OpenCV libraries
target_link_libraries(test PRIVATE ${OpenCV_LIBS})

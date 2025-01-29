cmake_minimum_required(VERSION 3.30)
project(RendererProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# Specify the path to the SFML libraries
set(SFML_DIR /opt/homebrew/Cellar/sfml/2.6.2/lib/cmake/SFML)

# Find the SFML package (dynamic linking)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add the executable
add_executable(RendererProject
        src/main.cpp
        src/InputHandler.cpp
        src/InputHandler.h
        src/LinAlg.cpp
        src/LinAlg.h
        src/Rasterizer.cpp
        src/Rasterizer.h)


# Link SFML dynamically
target_link_libraries(RendererProject PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)

# Add a post-build step to copy .dylib files to the libs folder
add_custom_command(TARGET RendererProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:RendererProject>/../libs
        COMMAND ${CMAKE_COMMAND} -E copy
        /opt/homebrew/Cellar/sfml/2.6.2/lib/libsfml-graphics.2.6.2.dylib
        /opt/homebrew/Cellar/sfml/2.6.2/lib/libsfml-window.2.6.2.dylib
        /opt/homebrew/Cellar/sfml/2.6.2/lib/libsfml-system.2.6.2.dylib
        $<TARGET_FILE_DIR:RendererProject>/../libs
)

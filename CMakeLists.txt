cmake_minimum_required(VERSION 3.15)
project(first_programs)

set(CMAKE_CXX_STANDARD 17)

# Public include directories
include_directories(common include include/glad include/GLFW include/glm include/KNR)

# Detect OS
if (WIN32)
    message(STATUS ">>> Building on Windows <<<")
    # glfw-3.4.bin.WIN64 is in D:\. Change accordingly if any changes take place.
    set(GLFW_DIR "D:/glfw-3.4.bin.WIN64")
    include_directories(${GLFW_DIR}/include)
    link_directories(${GLFW_DIR}/lib-mingw-w64)  # If using MSVC, change to lib-vc2022 or lib-vc2023

elseif (APPLE)
    message(STATUS ">>> Building on macOS <<<")
    # GLFW & GLM installed via Homebrew in macOS
    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)
endif ()

# Find OpenGL
find_package(OpenGL REQUIRED)

# General GLAD/GLFW wrapper
# This way, COMMON_SRC = ["common/glad.c", "common/wrapper_glfw.cpp", "common/wrapper_glfw.h"]
set(COMMON_SRC
        common/glad.c
        common/wrapper_glfw.cpp
        common/wrapper_glfw.h
)

# Three separate .cpp demos so a target is generated for each one

# === basic ===
add_executable(basic ${COMMON_SRC} graphics_examples/basic/basic.cpp)
target_link_libraries(basic PRIVATE ${OPENGL_LIBRARIES} glfw)

# Extra libraries based on different OS
if (APPLE)
    target_link_libraries(basic
            PRIVATE
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreFoundation"
            "-framework CoreGraphics"
            "-framework AppKit"
            "-framework CoreVideo"
    )
elseif (WIN32)
    #    target_link_libraries(basic
    #            PRIVATE
    #            opengl32
    #    )
endif ()


# === basic_wrapper ===
add_executable(basic_wrapper ${COMMON_SRC}
        graphics_examples/basic_wrapper/basic.cpp
        graphics_examples/basic_wrapper/basic.vert
        graphics_examples/basic_wrapper/basic.frag
)
target_link_libraries(basic_wrapper PRIVATE ${OPENGL_LIBRARIES} glfw3)

# Extra libraries based on different OS
if (APPLE)
    target_link_libraries(basic_wrapper
            PRIVATE
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreFoundation"
            "-framework CoreGraphics"
            "-framework AppKit"
            "-framework CoreVideo"
    )
elseif (WIN32)
    #    target_link_libraries(basic
    #            PRIVATE
    #            opengl32
    #    )
endif ()

# Copy basic_wrapper shader files to the build directory (cmake-build-debug)
file(COPY
        graphics_examples/basic_wrapper/basic.vert
        graphics_examples/basic_wrapper/basic.frag
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

# === vertex_attribs ===
add_executable(vertex_attribs ${COMMON_SRC} graphics_examples/vertex_attribs/vertex_attribs.cpp)
target_link_libraries(vertex_attribs PRIVATE ${OPENGL_LIBRARIES} glfw3)

# Extra libraries based on different OS
if (APPLE)
    target_link_libraries(vertex_attribs
            PRIVATE
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreFoundation"
            "-framework CoreGraphics"
            "-framework AppKit"
            "-framework CoreVideo"
    )
elseif (WIN32)
    #    target_link_libraries(basic
    #            PRIVATE
    #            opengl32
    #    )
endif ()

# Copy vertex_attribs shader files to the build directory (cmake-build-debug)
file(COPY
        graphics_examples/vertex_attribs/vert_attrib.vert
        graphics_examples/vertex_attribs/vert_attrib.frag
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

# Link OpenGL, GLFW target
# This usually needs to be put at the end:
#target_link_libraries(CONFIGURATION_NAME
#        ${OPENGL_LIBRARIES}
#        glfw
#)
# opengl-demo

Used for DI41008 - Graphics module self-learning purpose. **WARNING: DIICSU holds copyright on certain code. Use
responsibly.**

This project is based on example code provided and modified for macOS (Apple Silicon) and Windows 11 compatibility. I
personally use CLion because I think Visual Studio sucks lmao

Some noteworthy points in CMake file:

**Linking GLFW on Windows 11 (should work for Windows 10 as well)**

```cmake
set(GLFW_DIR "YOUR_LOCATION_TO/glfw-3.4.bin.WIN64")
include_directories(${GLFW_DIR}/include)
link_directories(${GLFW_DIR}/lib-mingw-w64)  # If using MSVC, change to lib-vc2022 or lib-vc2023
```

For GLFW on Windows, download at https://www.glfw.org/download.html (Windows pre-compiled libraries). Change the path
accordingly. If not using MinGW as toolchain, change to the correct lib directory.

**Linking GLFW on macOS (Apple Silicon)**

```cmake
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)
```

GLFW can be installed on Mac via Homebrew, so it is assumed here. If it is downloaded online, follow the method
described before.

**Extra libraries for macOS**

Somehow when linking `glfw3` library instead of `glfw` on Mac, a bunch of "undefined symbols" errors can occur. This can be
solved by linking some extra libraries:

```cmake
target_link_libraries(YOUR_CONFIGURATION_NAME
        PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework CoreGraphics"
        "-framework AppKit"
        "-framework CoreVideo"
)
```

By WaterCoFire, last updated 17 Oct 2025

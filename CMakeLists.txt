cmake_minimum_required(VERSION 3.0.0)
project(Lab1 VERSION 0.1.0)

# Use c++11
set(CMAKE_CXX_STANDARD 17)

# Header
set(GLAD_H ${PROJECT_SOURCE_DIR}/include)
set(GLFW_H /usr/local/include)
set(GLM_H /Users/manmantou/Documents/Mylib/glm)
include_directories(${GLAD_H} ${GLFW_H} ${GLM_H} "${PROJECT_SOURCE_DIR}/include")

# Link
set(GLFW_LINK /usr/local/lib/libglfw.3.dylib)
set(ASSIMP_LINK /usr/local/lib/libassimp.4.0.1.dylib)
link_libraries(${GLFW_LINK} ${ASSIMP_LINK})

# Executable output
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin") 

# Compile
# set(SOURCES glad.c main.cpp)
add_executable(Lab  # 输出名为chess的可执行文件
   ./src/main.cpp
   ./src/glad.c
   ./src/Camera.cpp
   ./src/Shader.cpp
   ./src/stb_image.cpp
   ./src/Player.cpp
   ./src/imgui.cpp
   ./src/imgui_widgets.cpp
   ./src/imgui_tables.cpp
   ./src/imgui_draw.cpp
   ./src/imgui_demo.cpp
   ./src/imgui_impl_glfw.cpp
   ./src/imgui_impl_opengl3.cpp
)

# Link the OpenGL framework in system
if (APPLE)
    target_link_libraries(Lab "-framework OpenGL")
endif()

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

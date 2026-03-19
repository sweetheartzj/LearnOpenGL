cmake_minimum_required(VERSION 4.1)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)

# 添加 GLFW 子目录
add_subdirectory(extern/glfw)

# 创建一个静态库目标，包含 glad.c
add_library(glad STATIC extern/glad/src/glad.c)
# 指定 glad 的头文件目录（这样包含 #include <glad/glad.h> 时可以找到）
target_include_directories(glad PUBLIC extern/glad/include)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE extern/stb)

add_subdirectory(extern/glm)

include_directories(include)

# 创建可执行文件
add_executable(LearnOpenGL
        main.cpp
        src/Shader.cpp
)

target_link_libraries(LearnOpenGL PRIVATE glfw glad stb glm)

# 添加 shaders 目录到编译目录
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})


# We will always try to use a version > 3.1 if avaliable
cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
   set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

project(opengl_study)

# Here we set the C++ standard to use
set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)


add_executable(openglStudy)
target_include_directories(openglStudy PRIVATE include)

target_sources(openglStudy
        PRIVATE
        src/openGLFW.cpp
        src/Shader.cpp
        src/ffImage.cpp
        src/Camera.cpp
        include/Base.h
        include/Shader.h
        include/stb_image.h
        include/ffImage.h
        include/Camera.h
   )

target_link_libraries(openglStudy
        PRIVATE
        glfw
        glad::glad
        glm::glm)

# Copy the shaders to the build directory
add_custom_target(openglStudyCopyShaders ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory
   ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders
   $<TARGET_FILE_DIR:openglStudy>/shaders
)

add_custom_target(openglStudyCopyTextures ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory
   ${CMAKE_CURRENT_SOURCE_DIR}/src/textures
   $<TARGET_FILE_DIR:openglStudy>/textures
)
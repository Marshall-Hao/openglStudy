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

add_executable(openglFrameBuffer)
target_include_directories(openglFrameBuffer PRIVATE frameBuffer/include)

target_sources(openglFrameBuffer
        PRIVATE
        frameBuffer/openGLFW.cpp
        frameBuffer/Shader.cpp
        frameBuffer/ffImage.cpp
        frameBuffer/Camera.cpp
        include/Base.h
        include/Shader.h
        include/stb_image.h
        include/ffImage.h
        include/Camera.h
   )

target_link_libraries(openglFrameBuffer
        PRIVATE
        glfw
        glad::glad
        glm::glm)

 # Copy the shaders to the build directory
add_custom_target(openglFrameBufferCopyShaders ALL
COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/frameBuffer/shader
$<TARGET_FILE_DIR:openglFrameBuffer>/shader
)

add_custom_target(openglFrameBufferCopyRes ALL
COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/frameBuffer/res
$<TARGET_FILE_DIR:openglFrameBuffer>/res
)

add_executable(openglCubeMap)
target_include_directories(openglCubeMap PRIVATE openglCubeMap/include)

target_sources(openglCubeMap
        PRIVATE
        openglCubeMap/openGLFW.cpp
        openglCubeMap/Shader.cpp
        openglCubeMap/ffImage.cpp
        openglCubeMap/Camera.cpp
        include/Base.h
        include/Shader.h
        include/stb_image.h
        include/ffImage.h
        include/Camera.h
   )

target_link_libraries(openglCubeMap
        PRIVATE
        glfw
        glad::glad
        glm::glm)


 # Copy the shaders to the build directory
add_custom_target(openglCubeMapCopyShaders ALL
COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/openglCubeMap/shader
$<TARGET_FILE_DIR:openglCubeMap>/shader
)



add_executable(advanceGLSL)
target_include_directories(advanceGLSL PRIVATE advanceGLSL/include)

target_sources(advanceGLSL
        PRIVATE
        advanceGLSL/openGLFW.cpp
        advanceGLSL/Shader.cpp
        advanceGLSL/ffImage.cpp
        advanceGLSL/Camera.cpp
        include/Base.h
        include/Shader.h
        include/stb_image.h
        include/ffImage.h
        include/Camera.h
   )

target_link_libraries(advanceGLSL
        PRIVATE
        glfw
        glad::glad
        glm::glm)

 # Copy the shaders to the build directory
 add_custom_target(advanceGLSLCopyShaders ALL
 COMMAND ${CMAKE_COMMAND} -E copy_directory
 ${CMAKE_CURRENT_SOURCE_DIR}/advanceGLSL/shader
 $<TARGET_FILE_DIR:advanceGLSL>/shader
 )

add_executable(GeometryShader)
target_include_directories(GeometryShader PRIVATE GeometryShader/include)

target_sources(GeometryShader
        PRIVATE
        GeometryShader/openGLFW.cpp
        GeometryShader/Shader.cpp
        GeometryShader/ffImage.cpp
        GeometryShader/Camera.cpp
        include/Base.h
        include/Shader.h
        include/stb_image.h
        include/ffImage.h
        include/Camera.h
   )

target_link_libraries(GeometryShader
        PRIVATE
        glfw
        glad::glad
        glm::glm)

 # Copy the shaders to the build directory
 add_custom_target(GeometryShaderCopyShaders ALL
 COMMAND ${CMAKE_COMMAND} -E copy_directory
 ${CMAKE_CURRENT_SOURCE_DIR}/GeometryShader/shader
 $<TARGET_FILE_DIR:GeometryShader>/shader
 )
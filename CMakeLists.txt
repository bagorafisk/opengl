cmake_minimum_required(VERSION 3.10)
project(opengl VERSION 1.0 LANGUAGES C CXX)

# —————————————————————————————
# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# —————————————————————————————
# Paths
set(SRC_DIR        ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR    ${CMAKE_SOURCE_DIR}/include)
set(VENDOR_INCLUDE ${CMAKE_SOURCE_DIR}/vendor/include)
set(VENDOR_LIB     ${CMAKE_SOURCE_DIR}/vendor/lib)

# —————————————————————————————
# GLAD: build from source
add_library(glad STATIC
    ${SRC_DIR}/glad.c
)
# glad needs its headers under vendor/include/glad
target_include_directories(glad
    PUBLIC
        ${VENDOR_INCLUDE}
)

# —————————————————————————————
# GLFW: import your prebuilt static library
add_library(glfw3 STATIC IMPORTED)
set_target_properties(glfw3 PROPERTIES
    IMPORTED_LOCATION ${VENDOR_LIB}/libglfw3.a
    # headers live in vendor/include/GLFW
    INTERFACE_INCLUDE_DIRECTORIES ${VENDOR_INCLUDE}
)

# —————————————————————————————
# GLM: header‑only
add_library(glm INTERFACE)
target_include_directories(glm
    INTERFACE
        ${VENDOR_INCLUDE}/glm
)

# —————————————————————————————
# Your executable
set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/shader.cpp
    ${SRC_DIR}/VAO.cpp
    ${SRC_DIR}/VBO.cpp
    ${SRC_DIR}/EBO.cpp
    ${SRC_DIR}/texture.cpp
    ${SRC_DIR}/stb_image.cpp
    ${SRC_DIR}/camera.cpp
    ${SRC_DIR}/mesh.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})

# make sure your own headers and all vendor headers are visible
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${INCLUDE_DIR}
        ${VENDOR_INCLUDE}
)

# —————————————————————————————
# Link everything together
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glad
        glfw3
        glm
        OpenGL::GL
)

# on Unix you'll generally need pthread, dl and math
if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            Threads::Threads
            dl
            m
    )
endif()

# Copy shader files to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    COMMENT "Copying shaders to build directory"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:${PROJECT_NAME}>/textures
    COMMENT "Copying textures to build directory"
)
cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(project_name opengl)

project(${project_name})

include_directories(libraries/freetype/include)
find_library(FreetypeLib freetype "libraries/freetype/release dll/win64")

add_executable(
    ${project_name}
        source/main.cpp

        source/Controllers/Initializer.h
        source/Controllers/Initializer.cpp
        source/Controllers/EventSystem.h
        source/Controllers/EventSystem.cpp
        source/Controllers/LightingController.h
        source/Controllers/LightingController.cpp

        source/Shaders/Shader.h
        source/Shaders/Shader.cpp

        source/Components/Component.cpp
        source/Components/Component.h
        source/Components/TransformComponent.cpp
        source/Components/TransformComponent.h
        source/Components/CameraComponent.cpp
        source/Components/CameraComponent.h
        source/Components/TextRendererComponent.cpp
        source/Components/TextRendererComponent.h

        source/GameObject/GameObject.cpp
        source/GameObject/GameObject.h
        source/GameObject/ObjectsManager.cpp
        source/GameObject/ObjectsManager.h

        source/Math/Vectors.cpp
        source/Math/Vectors.h
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(libraries/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(libraries/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(libraries/glm)
target_link_libraries(${PROJECT_NAME} glm)

target_link_libraries(${PROJECT_NAME} ${FreetypeLib})
target_include_directories(${PROJECT_NAME} PRIVATE "libraries/freetype/include")
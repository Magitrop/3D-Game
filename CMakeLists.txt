cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(project_name opengl)

project(${project_name})

include_directories(libraries/freetype/include)
find_library(FreetypeLib freetype "libraries/freetype/release dll/win64")

include_directories(libraries/assimp/include)
find_library(AssimpLib assimp "libraries/assimp/lib/Debug")

add_executable(
    ${project_name}
        source/main.cpp

        source/Mesh.cpp
        source/Mesh.h
        source/Model.cpp
        source/Model.h

        source/Controllers/Initializer.h
        source/Controllers/Initializer.cpp
        source/Controllers/EventSystem.h
        source/Controllers/EventSystem.cpp
        source/Controllers/LightingController.h
        source/Controllers/LightingController.cpp
        source/Controllers/ResourceManager.h
        source/Controllers/ResourceManager.cpp

        source/Shaders/Shader.h
        source/Shaders/Shader.cpp

        source/Components/Component.cpp
        source/Components/Component.h
        source/Components/RendererComponentBase.cpp
        source/Components/RendererComponentBase.h
        source/Components/TransformComponent.cpp
        source/Components/TransformComponent.h
        source/Components/CameraComponent.cpp
        source/Components/CameraComponent.h
        source/Components/TextRendererComponent.cpp
        source/Components/TextRendererComponent.h
        source/Components/ModelRendererComponent.cpp
        source/Components/ModelRendererComponent.h

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

target_link_libraries(${PROJECT_NAME} ${AssimpLib})
target_include_directories(${PROJECT_NAME} PRIVATE "libraries/assimp/include")
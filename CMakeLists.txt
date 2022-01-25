cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(project_name opengl)

project(${project_name})

add_executable(
    ${project_name}
        source/main.cpp

        source/Initializer.h

        source/Components/Component.cpp
        source/Components/Component.h
        source/Components/TransformComponent.cpp
        source/Components/TransformComponent.h

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
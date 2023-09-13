cmake_minimum_required(VERSION 3.7)
project(Moirai)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Moirai/Vendor/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(Moirai/Vendor/assimp)

add_subdirectory(Moirai/Vendor/entt)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++17")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(Moirai/Headers/
                    Moirai/Vendor/assimp/include/
                    Moirai/Vendor/glad/include/
                    Moirai/Vendor/glfw/include/
                    Moirai/Vendor/glm/
                    Moirai/Vendor/stb/
		            Moirai/Vendor/imgui/
                    Moirai/Vendor/entt/src)

file(GLOB VENDORS_SOURCES Moirai/Vendor/glad/src/glad.c
			  Moirai/Vendor/imgui/*.cpp
			  Moirai/Vendor/imgui/*.h
			  Moirai/Vendor/imgui/backends/imgui_impl_glfw.cpp
              Moirai/Vendor/imgui/backends/imgui_impl_glfw.h
			  Moirai/Vendor/imgui/backends/imgui_impl_opengl3.cpp
			  Moirai/Vendor/imgui/backends/imgui_impl_opengl3.h)

file(GLOB PROJECT_SRC Moirai/Src/*.cpp
                      Moirai/Src/*.h
		      Moirai/Src/Components/*.cpp
		      Moirai/Src/Components/*.h
		      Moirai/Src/Systems/*.cpp
		      Moirai/Src/Systems/*.h
		      Moirai/Src/Renderer/*.cpp
		      Moirai/Src/Renderer/*.h)

file(GLOB PROJECT_SHADERS Moirai/Shaders/*.comp
                          Moirai/Shaders/*.frag
                          Moirai/Shaders/*.geom
                          Moirai/Shaders/*.vert)

file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Src" FILES ${PROJECT_SRC})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SRC} ${PROJECT_SHADERS} 
                               ${PROJECT_CONFIGS} ${VENDORS_SOURCES})
target_link_libraries(${PROJECT_NAME} assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Moirai/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_SHADERS})

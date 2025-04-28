# TO BYŁO WCZEŚNIEJ, zerknąć

#cmake_minimum_required(VERSION 3.30)
#project(patrykcpp)
#
#set(CMAKE_CXX_STANDARD 20)
#
## znajduje wymagane pakiety
#find_package(imgui REQUIRED)
#find_package(glfw3 REQUIRED)
#find_package(OpenGL REQUIRED)
#
## dodaje pliki źródłowe, w tym implementacje backendów
#add_executable(patrykcpp
#        src/main.cpp
#        src/imgui_backends/imgui_impl_glfw.cpp
#        src/imgui_backends/imgui_impl_opengl3.cpp
#)
#
## definicje preprocesora dla imgui
#target_compile_definitions(patrykcpp PRIVATE
#        IMGUI_IMPL_OPENGL_LOADER_GLAD
#)
#
## linkuje biblioteki
#target_link_libraries(patrykcpp PRIVATE
#        imgui::imgui
#        glfw
#        OpenGL::GL
#)
#
## notatka dla siebie: jeśli bym używał plików nagłówkowych i implementacji backendów z imgui:
## UPEWNNIĆ się, że katalogi są dostępne w ścieżce include
#target_include_directories(patrykcpp PRIVATE
#        ${imgui_INCLUDE_DIRS}
#        src/imgui_backends
#)

cmake_minimum_required(VERSION 3.30)
project(main)

set(CMAKE_CXX_STANDARD 20)

# znajduje wymagane pakiety
find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# =================== GŁÓWNY PROGRAM ===================
add_executable(${PROJECT_NAME}
        src/main.cpp
        src/imgui_backends/imgui_impl_glfw.cpp
        src/imgui_backends/imgui_impl_opengl3.cpp
        src/Enigma_Module.cpp
        src/Enigma_Module.h
        src/communications.cpp
        src/communications.h
        src/GUI.cpp
        src/GUI.h
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        IMGUI_IMPL_OPENGL_LOADER_GLAD
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        imgui::imgui
        glfw
        OpenGL::GL
        ws2_32 
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${imgui_INCLUDE_DIRS}
        src/imgui_backends
        src
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)




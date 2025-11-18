cmake_minimum_required(VERSION 3.15)
project(SolarSystem VERSION 1.0.0 LANGUAGES CXX)

# Configuración de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Establece el directorio de salida para los binarios
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Opciones de compilación
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Flags de compilación
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Encuentra raylib
find_package(raylib 5.0 QUIET)
if (NOT raylib_FOUND)
    message(STATUS "raylib no encontrado, descargando...")
    include(FetchContent)
    FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
    )
    FetchContent_MakeAvailable(raylib)
endif()

# Incluye directorios
include_directories(${CMAKE_SOURCE_DIR}/include)

# Ejecutable
add_executable(solar_system 
    src/main.cpp
    src/Fisicas.cpp
)

# Link raylib
target_link_libraries(solar_system PRIVATE raylib)

# Configuración específica de plataforma
if(APPLE)
    target_link_libraries(solar_system PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()

# Instalación
install(TARGETS solar_system DESTINATION bin)

# Información de compilación
message(STATUS "=== Configuración del Sistema Solar ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Binary output: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "========================================")
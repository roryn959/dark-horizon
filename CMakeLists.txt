cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(dark-horizon)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include/)

enable_language(OBJCXX)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.mm)

set(METAL_SRC ${CMAKE_SOURCE_DIR}/src/Execution/PhotonTracer.metal)
set(METAL_AIR ${CMAKE_BINARY_DIR}/PhotonTracer.air)
set(METAL_LIB ${CMAKE_BINARY_DIR}/PhotonTracer.metallib)

add_custom_command(
    OUTPUT ${METAL_LIB}
    COMMAND xcrun metal -c ${METAL_SRC} -o ${METAL_AIR}
    COMMAND xcrun metallib ${METAL_AIR} -o ${METAL_LIB}
    DEPENDS ${METAL_SRC}
)

add_custom_target(metal_shaders ALL DEPENDS ${METAL_LIB})

add_executable(main ${SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_dependencies(main metal_shaders)

target_link_libraries(main
    ${SDL2_LIBRARIES}
    "-framework Metal"
    "-framework Foundation"
)

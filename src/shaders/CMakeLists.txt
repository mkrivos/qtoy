cmake_minimum_required(VERSION 3.16)

find_package(glm REQUIRED)
find_package(gli REQUIRED)
find_package(blend2d REQUIRED)
find_package(Poco REQUIRED COMPONENTS Foundation)
find_package(OpenMP REQUIRED)

add_library(shaders SHARED
    shaders.cpp
)

target_link_libraries(shaders PRIVATE glm::glm gli)
target_include_directories(shaders PUBLIC ../)
target_link_libraries(shaders PRIVATE Poco::Foundation blend2d::blend2d)

set_source_files_properties(shaders.cpp PROPERTIES COMPILE_DEFINITIONS GLM_FORCE_SWIZZLE=1)

set_property(SOURCE shaders.cpp APPEND PROPERTY COMPILE_FLAGS "-O0 -mavx2 -ftree-vectorize -fopenmp")
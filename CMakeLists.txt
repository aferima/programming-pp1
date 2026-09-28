cmake_minimum_required(VERSION 3.10.0)

project(lab3 VERSION 0.1.0 LANGUAGES CXX)
if (MSVC)
    add_compile_options(/W3)
    add_compile_options(/std:c++23preview)
    add_compile_options(/EHsc)
    add_compile_options(/utf-8)
else()
    add_compile_options(-Wall)
    add_compile_options(-Wextra)
    add_compile_options(-Wpedantic)
    add_compile_options(-std=c++23)
    add_compile_options(-fexceptions)
endif()

add_executable(lab3
    main.cpp
    weighted_random.cpp
)
cmake_minimum_required(VERSION 3.10)
project(untitled11 C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled11 main.c
        headers/step3.h
        steps/step3.c
        headers/step2.h
        headers/step2.h
        steps/step2.c
        headers/macro.h
        steps/macro.c)

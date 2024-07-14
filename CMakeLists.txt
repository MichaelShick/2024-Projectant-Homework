cmake_minimum_required(VERSION 3.28)
project(untitled10 C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled10 server.c
        client.c
        client.h
        server.h
        server_main.c
        client_main.c
)

cmake_minimum_required(VERSION 3.20)


project(ChatRoom)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    server

    server/ClientConnection.cpp
    server/ClientManager.cpp
    server/Server.cpp
    server/Main.cpp
)

add_executable(
    client

    client/Client.cpp
    client/Main.cpp
)


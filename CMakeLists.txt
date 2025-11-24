cmake_minimum_required(VERSION 3.16)
project(LSA_2025)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fdiagnostics-color=always")

include_directories(
        Headers
        Sources
        .
)

add_executable(LSA_2025
        main.cpp
        Sources/In.cpp
        Sources/Parm.cpp
        Sources/Error.cpp
        Sources/Out.cpp
        Sources/MFST.cpp
        Sources/IT.cpp
        Sources/LexA.cpp
        Sources/LT.cpp
        Sources/SemA.cpp
        Sources/RPN.cpp
        Sources/Log.cpp
        Sources/FST.cpp
        Sources/GRB.cpp
        Sources/asmGenerator.cpp
)

target_sources(LSA_2025 PRIVATE
        Headers/In.h
        Headers/Parm.h
        Headers/Out.h
        Headers/Error.h
        Headers/MFST.h
        Headers/IT.h
        Headers/LexA.h
        Headers/LT.h
        Headers/SemA.h
        Headers/Rules.h
        Headers/RPN.h
        Headers/Log.h
        Headers/FST.h
        Headers/GRB.h
        Headers/asmGenerator.h
)
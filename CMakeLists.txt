cmake_minimum_required(VERSION 3.10)
project(LSA_2025)

set(SOURCES
#    asmGenerator.cpp
#    asmGenerator.h
    Error.cpp
    Error.h
    FST.cpp
    FST.h
    GRB.cpp
    GRB.h
    In.cpp
    In.h
    IT.cpp
    IT.h
        Lex.cpp
        Lex.h
    Log.cpp
    Log.h
    LT.cpp
    LT.h
    main.cpp
    MFST.cpp
    MFST.h
    Out.cpp
    Out.h
        Parm.cpp
        Parm.h
#    RPN.cpp
#    RPN.h
    Rules.h
#    SemA.cpp
#    SemA.h
    stdafx.h
)

add_executable(LSA_2025 ${SOURCES})

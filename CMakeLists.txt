cmake_minimum_required (VERSION 2.8.11)
project (BP)

ADD_CUSTOM_COMMAND(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
   COMMAND re2c -o ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.re
   DEPENDS src/lexer.re ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
)

ADD_CUSTOM_COMMAND(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/parser.h ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
   COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/parser.h ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.yy
   COMMAND lemon ${CMAKE_CURRENT_BINARY_DIR}/parser.yy
   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/parser.c ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.yy
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
        # C/C++ sources and headers
        src/lexer.h

        # generated C/C++ sources and headers
        ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/parser.h
)

ADD_EXECUTABLE(bp_lexer src/main.cpp ${SOURCES})
ADD_EXECUTABLE(bp_parser src/main_parser.cpp ${SOURCES})

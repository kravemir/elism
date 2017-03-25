cmake_minimum_required (VERSION 2.8.11)
project (BP)

ADD_CUSTOM_COMMAND(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
   COMMAND re2c -o ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/lexer.re
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/lexer.re ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
)

ADD_CUSTOM_COMMAND(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/parser.h ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
   COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/parser.h ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.yy
   COMMAND lemon ${CMAKE_CURRENT_BINARY_DIR}/parser.yy
   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/parser.c ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser.yy
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_EXECUTABLE(bp_lexer main.cpp lexer.cpp lexer.h)
ADD_EXECUTABLE(bp_parser main_parser.cpp lexer.cpp lexer.h)

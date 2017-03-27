cmake_minimum_required (VERSION 2.8.11)
project (BP)

# LLVM CONFIG
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs core mcjit native irreader)

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

        src/ast/ExprNode.h
        src/ast/ExprNode.cpp
        src/ast/IntValNode.h
        src/ast/IntValNode.cpp

        src/ast/TypeNode.cpp
        src/ast/TypeNode.h
        src/ast/NamedTypeNode.cpp
        src/ast/NamedTypeNode.h
        src/ast/ArrayTypeNode.cpp
        src/ast/ArrayTypeNode.h

        src/ast/StatementNode.h
        src/ast/StatementNode.cpp
        src/ast/ReturnNode.h
        src/ast/ReturnNode.cpp

        src/ast/FunctionNode.cpp
        src/ast/FunctionNode.h
        src/ast/Program.cpp
        src/ast/Program.h

        # generated C/C++ sources and headers
        ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/parser.h
)

ADD_LIBRARY(shared_lib STATIC ${SOURCES})

ADD_EXECUTABLE(bp_lexer src/main.cpp)
ADD_EXECUTABLE(bp_parser src/main_parser.cpp)
ADD_EXECUTABLE(bp_jit src/main_jit.cpp)

target_link_libraries(bp_lexer shared_lib)
target_link_libraries(bp_parser shared_lib)
target_link_libraries(bp_jit shared_lib ${llvm_libs} rt )
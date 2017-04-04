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

        src/ast/all.h

        src/ast/ExprNode.h
        src/ast/ExprNode.cpp
        src/ast/expressions/ConstIntExprNode.h
        src/ast/expressions/ConstIntExprNode.cpp
        src/ast/expressions/BinaryOperationExprNode.h
        src/ast/expressions/BinaryOperationExprNode.cpp
        src/ast/expressions/NameExprNode.h
        src/ast/expressions/NameExprNode.cpp
        src/ast/expressions/CallExprNode.h
        src/ast/expressions/CallExprNode.cpp
        src/ast/expressions/ChildExprNode.h
        src/ast/expressions/ChildExprNode.cpp

        src/ast/TypeNode.cpp
        src/ast/TypeNode.h
        src/ast/types/NamedTypeNode.cpp
        src/ast/types/NamedTypeNode.h
        src/ast/types/ArrayTypeNode.cpp
        src/ast/types/ArrayTypeNode.h

        src/ast/StatementNode.h
        src/ast/StatementNode.cpp
        src/ast/statements/ReturnStatementNode.h
        src/ast/statements/ReturnStatementNode.cpp
        src/ast/statements/LetStatementNode.h
        src/ast/statements/LetStatementNode.cpp
        src/ast/statements/VarStatementNode.h
        src/ast/statements/VarStatementNode.cpp
        src/ast/statements/AssignStatementNode.h
        src/ast/statements/AssignStatementNode.cpp

        src/ast/declarations/FunctionNode.h
        src/ast/declarations/FunctionNode.cpp
        src/ast/declarations/ClassNode.h
        src/ast/declarations/ClassNode.cpp
        src/ast/Program.cpp
        src/ast/Program.h

        src/CodegenContext.h
        src/CodegenContext.cpp

        # generated C/C++ sources and headers
        ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/parser.h src/codegen/IntType.cpp src/codegen/IntType.h)

ADD_LIBRARY(shared_lib STATIC ${SOURCES})

ADD_EXECUTABLE(bp_lexer src/main.cpp)
ADD_EXECUTABLE(bp_parser src/main_parser.cpp)
ADD_EXECUTABLE(bp_jit src/main_jit.cpp)

target_link_libraries(bp_lexer shared_lib ${llvm_libs} rt )
target_link_libraries(bp_parser shared_lib ${llvm_libs} rt )
target_link_libraries(bp_jit shared_lib ${llvm_libs} rt )
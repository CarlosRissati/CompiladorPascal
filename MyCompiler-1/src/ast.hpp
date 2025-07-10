// filepath: d:\Coisas\estudos\faculdade\ZeLuis\Compiladores\MyCompiler\MyCompiler\src\ast.hpp
#pragma once
#include <string>
#include <vector>

// Enum for different node types in the AST
enum class NodeType {
    PROGRAM,
    VARIABLE_DECLARATION,
    ASSIGNMENT,
    EXPRESSION,
    FUNCTION_DECLARATION,
    PROCEDURE_DECLARATION,
    LABEL_DECLARATION,
    TYPE_DECLARATION,
    CONSTANT_DECLARATION,
    BLOCK,
    IF_STATEMENT,
    WHILE_STATEMENT,
    FOR_STATEMENT,
    REPEAT_STATEMENT,
    CASE_STATEMENT,
    TRY_STATEMENT,
    CATCH_STATEMENT,
    FINALLY_STATEMENT,
    // Add more node types as needed
};

// Structure for a node in the AST
struct Node {
    NodeType type;                     // Type of the node
    std::string value;                 // Value associated with the node (e.g., variable name)
    std::vector<Node> children;        // Child nodes
};
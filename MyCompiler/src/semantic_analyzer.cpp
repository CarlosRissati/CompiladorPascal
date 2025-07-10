#include "semantic_analyzer.hpp"
#include <iostream>
#include <unordered_set>

SemanticAnalyzer::SemanticAnalyzer(const Node& root) : rootNode(root) {}

void SemanticAnalyzer::analyze() {
    // Start the semantic analysis from the root of the syntax tree
    checkDeclarations(rootNode);
    checkTypes(rootNode);
}

void SemanticAnalyzer::checkDeclarations(const Node& node) {
    // Example implementation for checking variable declarations
    std::unordered_set<std::string> declaredVariables;

    for (const auto& child : node.children) {
        if (child.type == NodeType::VARIABLE_DECLARATION) {
            const std::string& varName = child.value;
            if (declaredVariables.find(varName) != declaredVariables.end()) {
                std::cerr << "Error: Variable '" << varName << "' already declared." << std::endl;
            } else {
                declaredVariables.insert(varName);
            }
        }
        checkDeclarations(child); // Recursive call for child nodes
    }
}

void SemanticAnalyzer::checkTypes(const Node& node) {
    // Example implementation for type checking
    for (const auto& child : node.children) {
        if (child.type == NodeType::ASSIGNMENT) {
            const std::string& varName = child.value;
            // Assume we have a method to get the variable type
            Type varType = getVariableType(varName);
            Type assignedType = getExpressionType(child.children[1]); // Assuming the second child is the expression

            if (varType != assignedType) {
                std::cerr << "Error: Type mismatch in assignment to '" << varName << "'." << std::endl;
            }
        }
        checkTypes(child); // Recursive call for child nodes
    }
}

Type SemanticAnalyzer::getVariableType(const std::string& varName) {
    // Placeholder for getting the variable type
    return Type::UNKNOWN; // Replace with actual logic
}

Type SemanticAnalyzer::getExpressionType(const Node& expressionNode) {
    // Placeholder for determining the type of an expression
    return Type::UNKNOWN; // Replace with actual logic
}
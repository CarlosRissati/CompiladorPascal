#include "semantic_analyzer.hpp"
#include <iostream>
#include <unordered_set>

SemanticAnalyzer::SemanticAnalyzer(const Node& root) : rootNode(root) {}

void SemanticAnalyzer::analyze() {
    checkDeclarations(rootNode);
    checkTypes(rootNode);
}

void SemanticAnalyzer::checkDeclarations(const Node& node) {
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
        checkDeclarations(child);
    }
}

void SemanticAnalyzer::checkTypes(const Node& node) {
    for (const auto& child : node.children) {
        if (child.type == NodeType::ASSIGNMENT) {
            const std::string& varName = child.value;
            Type varType = getVariableType(varName);
            Type assignedType = getExpressionType(child.children[1]);

            if (varType != assignedType) {
                std::cerr << "Error: Type mismatch in assignment to '" << varName << "'." << std::endl;
            }
        }
        checkTypes(child);
    }
}

Type SemanticAnalyzer::getVariableType(const std::string& varName) {
    return Type::UNKNOWN; // Replace with actual logic
}

Type SemanticAnalyzer::getExpressionType(const Node& expressionNode) {
    return Type::UNKNOWN; // Replace with actual logic
}
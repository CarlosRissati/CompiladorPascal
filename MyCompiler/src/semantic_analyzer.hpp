#ifndef SEMANTIC_ANALYZER_HPP
#define SEMANTIC_ANALYZER_HPP

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include "parser.hpp"

class SemanticAnalyzer {
public:
    SemanticAnalyzer(const Node& root);
    void analyze();

private:
    const Node& root;
    std::unordered_map<std::string, std::string> symbolTable;

    void checkDeclarations(const Node& node);
    void checkTypes(const Node& node);
    void reportError(const std::string& message, const Node& node);
};

#endif // SEMANTIC_ANALYZER_HPP
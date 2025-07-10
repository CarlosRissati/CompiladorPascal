#ifndef SEMANTIC_ANALYZER_HPP
#define SEMANTIC_ANALYZER_HPP

#include "ast.hpp"
#include <string>

class SemanticAnalyzer {
public:
    explicit SemanticAnalyzer(const Node& root);
    void analyze();

private:
    void checkDeclarations(const Node& node);
    void checkTypes(const Node& node);
    Type getVariableType(const std::string& varName);
    Type getExpressionType(const Node& expressionNode);

    const Node& rootNode;
};

#endif // SEMANTIC_ANALYZER_HPP
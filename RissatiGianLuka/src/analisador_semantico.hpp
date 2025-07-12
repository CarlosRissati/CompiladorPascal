#ifndef ANALISADOR_SEMANTICO_HPP
#define ANALISADOR_SEMANTICO_HPP

#include "parser.hpp" 
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <stdexcept>

enum class SymbolType {
    UNKNOWN, INTEGER, REAL, BOOLEAN, STRING, PROCEDURE
};

struct Symbol {
    std::string name;
    SymbolType type;
    int line;
};

class SemanticAnalyzer {
public:
    void analyze(const NodePtr& root);

private:
    std::unordered_map<std::string, Symbol> symbolTable;
    std::unordered_set<std::string> forLoopControlVariables;

    void visit(const Node* node);
    void visit(const ProgramNode* node);
    void visit(const VarSectionNode* node);
    void visit(const BlockNode* node);
    void visit(const AssignNode* node);
    void visit(const IfNode* node);
    void visit(const WhileNode* node);
    void visit(const ForNode* node);
    void visit(const RepeatNode* node);

    SymbolType getExpressionType(const ExprNode* expr);
};

#endif
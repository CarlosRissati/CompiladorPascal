#include "analisador_semantico.hpp"

SymbolType stringToSymbolType(const std::string& typeName) {
    if (typeName == "integer") return SymbolType::INTEGER;
    if (typeName == "real") return SymbolType::REAL;
    if (typeName == "boolean") return SymbolType::BOOLEAN;
    if (typeName == "string") return SymbolType::STRING;
    return SymbolType::UNKNOWN;
}

std::string symbolTypeToString(SymbolType type) {
    switch(type) {
        case SymbolType::INTEGER: return "INTEGER";
        case SymbolType::REAL:    return "REAL";
        case SymbolType::BOOLEAN: return "BOOLEAN";
        case SymbolType::STRING:  return "STRING";
        default:                  return "UNKNOWN";
    }
}

void SemanticAnalyzer::analyze(const NodePtr& root) {
    if (!root) return;
    try {
        visit(root.get());
    } catch (const std::runtime_error& e) {
        std::cerr << "Erro durante a analise semantica: " << e.what() << std::endl;
    }
}

void SemanticAnalyzer::visit(const Node* node) {
    if (!node) return;

    if (auto p = dynamic_cast<const ProgramNode*>(node))       visit(p);
    else if (auto p = dynamic_cast<const VarSectionNode*>(node)) visit(p);
    else if (auto p = dynamic_cast<const BlockNode*>(node))      visit(p);
    else if (auto p = dynamic_cast<const AssignNode*>(node))     visit(p);
    else if (auto p = dynamic_cast<const IfNode*>(node))         visit(p);
    else if (auto p = dynamic_cast<const WhileNode*>(node))      visit(p);
    else if (auto p = dynamic_cast<const ForNode*>(node))        visit(p);
    else if (auto p = dynamic_cast<const RepeatNode*>(node))     visit(p);
}

void SemanticAnalyzer::visit(const ProgramNode* node) {
    if (node->vars) {
        visit(node->vars.get());
    }
    if (node->mainBlock) {
        visit(node->mainBlock.get());
    }
}

void SemanticAnalyzer::visit(const VarSectionNode* node) {
    for (const auto& entry : node->entries) {
        const std::string& varName = entry.first.value.value_or("");
        const std::string& typeName = entry.second.value.value_or("");

        if (symbolTable.count(varName)) {
            std::cerr << "Erro Semantico (linha " << entry.first.line << "): Variavel '" << varName << "' ja foi declarada." << std::endl;
        } else {
            symbolTable[varName] = {varName, stringToSymbolType(typeName), entry.first.line};
        }
    }
}

void SemanticAnalyzer::visit(const BlockNode* node) {
    for (const auto& stmt : node->statements) {
        visit(stmt.get());
    }
}

void SemanticAnalyzer::visit(const AssignNode* node) {
    const std::string& varName = node->target.value.value_or("");

    if (forLoopControlVariables.count(varName)) {
        std::cerr << "Erro Semantico (linha " << node->target.line << "): A variavel de controle do laco FOR '" << varName << "' nao pode ser modificada." << std::endl;
        return;
    }

    if (symbolTable.find(varName) == symbolTable.end()) {
        std::cerr << "Erro Semantico (linha " << node->target.line << "): Variavel '" << varName << "' nao foi declarada." << std::endl;
        return;
    }

    SymbolType varType = symbolTable.at(varName).type;
    SymbolType exprType = getExpressionType(node->value.get());

    if (varType != exprType && exprType != SymbolType::UNKNOWN) {
        std::cerr << "Erro Semantico (linha " << node->target.line << "): Incompatibilidade de tipos. Variavel '" << varName 
                  << "' e do tipo " << symbolTypeToString(varType) << " mas recebeu uma expressao do tipo " << symbolTypeToString(exprType) << "." << std::endl;
    }
}

void SemanticAnalyzer::visit(const IfNode* node) {
    SymbolType conditionType = getExpressionType(node->cond.get());
    if (conditionType != SymbolType::BOOLEAN && conditionType != SymbolType::UNKNOWN) {
        std::cerr << "Erro Semantico: A condicao do 'if' deve ser do tipo BOOLEAN." << std::endl;
    }
    visit(node->thenBr.get());
    if (node->elseBr) {
        visit(node->elseBr.get());
    }
}

void SemanticAnalyzer::visit(const WhileNode* node) {
    SymbolType conditionType = getExpressionType(node->cond.get());
    if (conditionType != SymbolType::BOOLEAN && conditionType != SymbolType::UNKNOWN) {
         std::cerr << "Erro Semantico: A condicao do 'while' deve ser do tipo BOOLEAN." << std::endl;
    }
    visit(node->body.get());
}

void SemanticAnalyzer::visit(const ForNode* node) {
    const std::string& varName = node->var.value.value_or("");

    if (!symbolTable.count(varName)) {
        std::cerr << "Erro Semantico (linha " << node->var.line << "): Variavel de controle do FOR '" << varName << "' nao foi declarada." << std::endl;
    } else {
        if (symbolTable.at(varName).type != SymbolType::INTEGER) {
            std::cerr << "Erro Semantico (linha " << node->var.line << "): Variavel de controle do FOR '" << varName << "' deve ser do tipo INTEGER." << std::endl;
        }
    }
    
    if (getExpressionType(node->start.get()) != SymbolType::INTEGER) {
        std::cerr << "Erro Semantico (linha " << node->var.line << "): A expressao inicial do FOR deve ser do tipo INTEGER." << std::endl;
    }
    if (getExpressionType(node->end.get()) != SymbolType::INTEGER) {
        std::cerr << "Erro Semantico (linha " << node->var.line << "): A expressao final do FOR deve ser do tipo INTEGER." << std::endl;
    }

    forLoopControlVariables.insert(varName);
    visit(node->body.get());
    forLoopControlVariables.erase(varName);
}

void SemanticAnalyzer::visit(const RepeatNode* node) {
    for (const auto& stmt : node->body) {
        visit(stmt.get());
    }
    SymbolType conditionType = getExpressionType(node->cond.get());
    if (conditionType != SymbolType::BOOLEAN && conditionType != SymbolType::UNKNOWN) {
        std::cerr << "Erro Semantico: A condicao do 'until' deve ser do tipo BOOLEAN." << std::endl;
    }
}

SymbolType SemanticAnalyzer::getExpressionType(const ExprNode* expr) {
    if (!expr) return SymbolType::UNKNOWN;

    if (auto lit = dynamic_cast<const LiteralNode*>(expr)) {
        switch (lit->value.type) {
            case Tipo_de_token::INT_LIT:    return SymbolType::INTEGER;
            case Tipo_de_token::REAL_LIT:   return SymbolType::REAL;
            case Tipo_de_token::STRING_LIT: return SymbolType::STRING;
            case Tipo_de_token::BOOL_LIT:   return SymbolType::BOOLEAN;
            default:                    return SymbolType::UNKNOWN;
        }
    }
    if (auto var = dynamic_cast<const IdentifierNode*>(expr)) {
        const std::string& varName = var->identifier.value.value_or("");
        if (symbolTable.count(varName)) {
            return symbolTable.at(varName).type;
        }
        std::cerr << "Erro Semantico (linha " << var->identifier.line << "): Variavel '" << varName << "' usada sem ser declarada." << std::endl;
        return SymbolType::UNKNOWN;
    }
    if (auto binOp = dynamic_cast<const BinaryOpNode*>(expr)) {
        SymbolType leftType = getExpressionType(binOp->left.get());
        SymbolType rightType = getExpressionType(binOp->right.get());

        if (binOp->op.type == Tipo_de_token::EQUAL || binOp->op.type == Tipo_de_token::NOT_EQUAL ||
            binOp->op.type == Tipo_de_token::LESS || binOp->op.type == Tipo_de_token::GREATER ||
            binOp->op.type == Tipo_de_token::LESS_EQUAL || binOp->op.type == Tipo_de_token::GREATER_EQUAL) {
            if (leftType == rightType && leftType != SymbolType::UNKNOWN) {
                return SymbolType::BOOLEAN;
            }
        } else if (leftType == rightType && (leftType == SymbolType::INTEGER || leftType == SymbolType::REAL)) {
            return leftType;
        }
        
        std::cerr << "Erro Semantico (linha " << binOp->op.line << "): Tipos incompativeis para o operador '" << binOp->op.value.value_or("") << "'." << std::endl;
        return SymbolType::UNKNOWN;
    }
    
    return SymbolType::UNKNOWN;
}
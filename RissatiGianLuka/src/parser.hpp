#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include "tokenization.hpp"

class Node;
class ExprNode;
class StmtNode;
class ProgramNode;
class ConstSectionNode;
class VarSectionNode;
class TypeSectionNode;
class FunctionDeclNode;
class BlockNode;
class IfNode;
class WhileNode;
class ForNode;
class RepeatNode;
class CaseNode;
class AssignNode;
class ProcCallNode;
class LiteralNode;
class IdentifierNode;
class BinaryOpNode;

class Node { public: virtual ~Node() = default; };
using NodePtr = std::unique_ptr<Node>;


class ExprNode : public Node { public: virtual ~ExprNode() = default; };
using ExprPtr = std::unique_ptr<ExprNode>;

class StmtNode : public Node { public: virtual ~StmtNode() = default; };
using StmtPtr = std::unique_ptr<StmtNode>;


class ProgramNode : public Node {
public:
    Token name;
    StmtPtr vars;
    StmtPtr mainBlock;
    ProgramNode(Token n, StmtPtr v, StmtPtr m)
      : name(std::move(n)), vars(std::move(v)), mainBlock(std::move(m)) {}
};

// Nó para a seção VAR
class VarSectionNode : public StmtNode {
public:
    std::vector<std::pair<Token, Token>> entries;
    VarSectionNode(std::vector<std::pair<Token, Token>> e) : entries(std::move(e)) {}
};

// Nó para um bloco BEGIN...END
class BlockNode : public StmtNode {
public:
    std::vector<StmtPtr> statements;
    BlockNode(std::vector<StmtPtr> stmts) : statements(std::move(stmts)) {}
};

// Nó para o comando IF-THEN-ELSE
class IfNode : public StmtNode {
public:
    ExprPtr cond;
    StmtPtr thenBr;
    StmtPtr elseBr;
    IfNode(ExprPtr c, StmtPtr t, StmtPtr e) : cond(std::move(c)), thenBr(std::move(t)), elseBr(std::move(e)) {}
};

// Nó para o laço WHILE
class WhileNode : public StmtNode {
public:
    ExprPtr cond;
    StmtPtr body;
    WhileNode(ExprPtr c, StmtPtr b) : cond(std::move(c)), body(std::move(b)) {}
};

// Nó para o laço FOR
class ForNode : public StmtNode {
public:
    Token var;
    ExprPtr start;
    ExprPtr end;
    bool toUp;
    StmtPtr body;
    ForNode(Token v, ExprPtr s, ExprPtr e, bool u, StmtPtr b) : var(std::move(v)), start(std::move(s)), end(std::move(e)), toUp(u), body(std::move(b)) {}
};

// Nó para o laço REPEAT...UNTIL
class RepeatNode : public StmtNode {
public:
    std::vector<StmtPtr> body;
    ExprPtr cond;
    RepeatNode(std::vector<StmtPtr> b, ExprPtr c) : body(std::move(b)), cond(std::move(c)) {}
};

// Nó para o comando de atribuição
class AssignNode : public StmtNode {
public:
    Token target;
    ExprPtr value;
    AssignNode(Token t, ExprPtr v) : target(std::move(t)), value(std::move(v)) {}
};

// Nó para um valor literal
class LiteralNode : public ExprNode {
public:
    Token value;
    LiteralNode(Token v) : value(std::move(v)) {}
};

// Nó para um identificador (uso de uma variável)
class IdentifierNode : public ExprNode {
public:
    Token identifier;
    IdentifierNode(Token id) : identifier(std::move(id)) {}
};

class BinaryOpNode : public ExprNode {
public:
    ExprPtr left;
    Token op;
    ExprPtr right;
    BinaryOpNode(ExprPtr l, Token o, ExprPtr r) : left(std::move(l)), op(std::move(o)), right(std::move(r)) {}
};


class Parser {
public:
    Parser(const std::vector<Token>& toks) : tokens(toks), pos(0) {}

    std::unique_ptr<ProgramNode> parseProgram() {
        try {
            return program();
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            return nullptr;
        }
    }

private:
    const std::vector<Token>& tokens;
    size_t pos;

    const Token& peek(int offset = 0) const {
        if (pos + offset >= tokens.size()) throw std::runtime_error("Fim inesperado do arquivo.");
        return tokens[pos + offset];
    }
    const Token& advance() {
        if (pos >= tokens.size()) throw std::runtime_error("Fim inesperado do arquivo ao avancar.");
        return tokens[pos++];
    }
    bool match(Tipo_de_token type) {
        if (pos >= tokens.size() || tokens[pos].type != type) return false;
        pos++;
        return true;
    }
    const Token& expect(Tipo_de_token type, const std::string& msg) {
        if (pos >= tokens.size() || tokens[pos].type != type) {
            std::string errmsg = "Erro sintatico: " + msg;
            if(pos < tokens.size()){
                errmsg += " (esperado '" + TokenTypeToString(type) + "', encontrado '" + TokenTypeToString(peek().type) + "' na linha " + std::to_string(peek().line) + ")";
            }
            throw std::runtime_error(errmsg);
        }
        return tokens[pos++];
    }

    // Métodos de parsing para cada regra da gramática
    std::unique_ptr<ProgramNode> program();
    StmtPtr parseVarDecl();
    StmtPtr parseBlock();
    StmtPtr parseStatement();
    StmtPtr parseIf();
    StmtPtr parseWhile();
    StmtPtr parseFor();
    StmtPtr parseRepeat();
    StmtPtr parseAssignment();

    ExprPtr parseExpression();
    ExprPtr parseRelational();
    ExprPtr parseAdditive();
    ExprPtr parseMultiplicative();
    ExprPtr parsePrimary();

    std::string TokenTypeToString(Tipo_de_token type); 
};


inline std::string Parser::TokenTypeToString(Tipo_de_token type) {
    switch (type) {
        case Tipo_de_token::PROGRAM: return "PROGRAM"; case Tipo_de_token::VAR: return "VAR";
        case Tipo_de_token::BEGIN: return "BEGIN"; case Tipo_de_token::END: return "END";
        case Tipo_de_token::INTEGER: return "INTEGER"; case Tipo_de_token::REAL: return "REAL";
        case Tipo_de_token::ASSIGN: return ":="; case Tipo_de_token::SEMICOLON: return ";";
        case Tipo_de_token::DOT: return "."; case Tipo_de_token::IDENTIFIER: return "identificador";
        default: return "TOKEN_DESCONHECIDO";
    }
}

inline std::unique_ptr<ProgramNode> Parser::program() {
    expect(Tipo_de_token::PROGRAM, "Esperado 'program' no inicio do arquivo.");
    Token name = expect(Tipo_de_token::IDENTIFIER, "Esperado nome do programa.");
    expect(Tipo_de_token::SEMICOLON, "Esperado ';' apos nome do programa.");
    
    StmtPtr vars = nullptr;
    if (peek().type == Tipo_de_token::VAR) {
        vars = parseVarDecl();
    }
    
    auto mainBlock = parseBlock();
    expect(Tipo_de_token::DOT, "Esperado '.' no fim do programa.");
    
    return std::make_unique<ProgramNode>(name, std::move(vars), std::move(mainBlock));
}

inline StmtPtr Parser::parseVarDecl() {
    expect(Tipo_de_token::VAR, "Esperado 'var'.");
    std::vector<std::pair<Token, Token>> entries;
    while (peek().type == Tipo_de_token::IDENTIFIER) {
        std::vector<Token> idList;
        idList.push_back(expect(Tipo_de_token::IDENTIFIER, "Esperado identificador."));
        while (match(Tipo_de_token::COMMA)) {
            idList.push_back(expect(Tipo_de_token::IDENTIFIER, "Esperado identificador apos a virgula."));
        }
        expect(Tipo_de_token::COLON, "Esperado ':' apos a lista de identificadores.");
        Token type = advance();
        expect(Tipo_de_token::SEMICOLON, "Esperado ';' apos a declaracao de tipo.");
        
        for (const auto& id : idList) {
            entries.emplace_back(id, type);
        }
    }
    return std::make_unique<VarSectionNode>(std::move(entries));
}


inline StmtPtr Parser::parseBlock() {
    expect(Tipo_de_token::BEGIN, "Esperado 'begin' para iniciar um bloco.");
    std::vector<StmtPtr> stmts;
    while (peek().type != Tipo_de_token::END) {
        stmts.push_back(parseStatement());
    }
    expect(Tipo_de_token::END, "Esperado 'end' para finalizar um bloco.");
    return std::make_unique<BlockNode>(std::move(stmts));
}

inline StmtPtr Parser::parseStatement() {
    StmtPtr stmt;
    switch(peek().type) {
        case Tipo_de_token::BEGIN:
            stmt = parseBlock();
            expect(Tipo_de_token::SEMICOLON, "Esperado ';' apos o bloco 'end'.");
            break;
        case Tipo_de_token::IDENTIFIER:
            stmt = parseAssignment();
            break;
        case Tipo_de_token::IF:
            stmt = parseIf();
            break;
        case Tipo_de_token::WHILE:
            stmt = parseWhile();
            break;
        case Tipo_de_token::FOR:
            stmt = parseFor();
            break;
        case Tipo_de_token::REPEAT:
            stmt = parseRepeat();
            break;
        default:
            throw std::runtime_error("Comando invalido ou inesperado na linha " + std::to_string(peek().line));
    }
    return stmt;
}

inline StmtPtr Parser::parseAssignment() {
    Token target = expect(Tipo_de_token::IDENTIFIER, "Esperado identificador para atribuicao.");
    expect(Tipo_de_token::ASSIGN, "Esperado ':=' para atribuicao.");
    auto value = parseExpression();
    expect(Tipo_de_token::SEMICOLON, "Esperado ';' no final do comando de atribuicao.");
    return std::make_unique<AssignNode>(target, std::move(value));
}

inline ExprPtr Parser::parseExpression() {
    return parseRelational();
}

inline ExprPtr Parser::parseRelational() {
    auto left = parseAdditive();
    while (peek().type == Tipo_de_token::EQUAL || peek().type == Tipo_de_token::NOT_EQUAL ||
           peek().type == Tipo_de_token::LESS || peek().type == Tipo_de_token::GREATER ||
           peek().type == Tipo_de_token::LESS_EQUAL || peek().type == Tipo_de_token::GREATER_EQUAL) {
        Token op = advance();
        auto right = parseAdditive();
        left = std::make_unique<BinaryOpNode>(std::move(left), op, std::move(right));
    }
    return left;
}

inline ExprPtr Parser::parseAdditive() {
    auto left = parseMultiplicative();
    while (peek().type == Tipo_de_token::PLUS || peek().type == Tipo_de_token::MINUS) {
        Token op = advance();
        auto right = parseMultiplicative();
        left = std::make_unique<BinaryOpNode>(std::move(left), op, std::move(right));
    }
    return left;
}

inline ExprPtr Parser::parseMultiplicative() {
    auto left = parsePrimary();
    while (peek().type == Tipo_de_token::MULTIPLY || peek().type == Tipo_de_token::DIVIDE || peek().type == Tipo_de_token::DIV) {
        Token op = advance();
        auto right = parsePrimary();
        left = std::make_unique<BinaryOpNode>(std::move(left), op, std::move(right));
    }
    return left;
}

inline ExprPtr Parser::parsePrimary() {
    if (peek().type == Tipo_de_token::INT_LIT || peek().type == Tipo_de_token::REAL_LIT ||
        peek().type == Tipo_de_token::STRING_LIT || peek().type == Tipo_de_token::BOOL_LIT) {
        return std::make_unique<LiteralNode>(advance());
    }
    if (peek().type == Tipo_de_token::IDENTIFIER) {
        return std::make_unique<IdentifierNode>(advance());
    }
    if (match(Tipo_de_token::OPEN_PAREN)) {
        auto expr = parseExpression();
        expect(Tipo_de_token::CLOSE_PAREN, "Esperado ')' para fechar expressao.");
        return expr;
    }
    throw std::runtime_error("Expressao primaria inesperada na linha " + std::to_string(peek().line));
}

inline StmtPtr Parser::parseIf() {
    expect(Tipo_de_token::IF, "");
    auto cond = parseExpression();
    expect(Tipo_de_token::THEN, "Esperado 'then' apos a condicao do 'if'.");
    auto thenBr = parseStatement();
    StmtPtr elseBr = nullptr;
    if (match(Tipo_de_token::ELSE)) {
        elseBr = parseStatement();
    }
    return std::make_unique<IfNode>(std::move(cond), std::move(thenBr), std::move(elseBr));
}

inline StmtPtr Parser::parseWhile() {
    expect(Tipo_de_token::WHILE, "");
    auto cond = parseExpression();
    expect(Tipo_de_token::DO, "Esperado 'do' no laco 'while'.");
    auto body = parseStatement();
    return std::make_unique<WhileNode>(std::move(cond), std::move(body));
}

inline StmtPtr Parser::parseFor() {
    expect(Tipo_de_token::FOR, "");
    Token var = expect(Tipo_de_token::IDENTIFIER, "Esperado variavel de controle para o 'for'.");
    expect(Tipo_de_token::ASSIGN, "Esperado ':=' no laco 'for'.");
    auto start = parseExpression();
    bool toUp = (peek().type == Tipo_de_token::TO);
    if (!toUp) expect(Tipo_de_token::DOWNTO, "Esperado 'to' ou 'downto'.");
    else advance();
    auto end = parseExpression();
    expect(Tipo_de_token::DO, "Esperado 'do' no laco 'for'.");
    auto body = parseStatement();
    return std::make_unique<ForNode>(var, std::move(start), std::move(end), toUp, std::move(body));
}

inline StmtPtr Parser::parseRepeat() {
    expect(Tipo_de_token::REPEAT, "");
    std::vector<StmtPtr> stmts;
    do {
        stmts.push_back(parseStatement());
    } while (peek().type != Tipo_de_token::UNTIL);
    expect(Tipo_de_token::UNTIL, "");
    auto cond = parseExpression();
    expect(Tipo_de_token::SEMICOLON, "Esperado ';' apos o 'repeat...until'.");
    return std::make_unique<RepeatNode>(std::move(stmts), std::move(cond));
}

#endif
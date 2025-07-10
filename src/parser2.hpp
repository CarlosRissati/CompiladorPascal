#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include "tokenization.hpp"

// AST node definitions (inline)
class Node { public: virtual ~Node() = default; };
using NodePtr = std::unique_ptr<Node>;

class ExprNode : public Node { public: virtual ~ExprNode() = default; };
using ExprPtr = std::unique_ptr<ExprNode>;

class StmtNode : public Node { public: virtual ~StmtNode() = default; };
using StmtPtr = std::unique_ptr<StmtNode>;

// Specific AST nodes
class ProgramNode : public Node {
public:
    Token name;
    StmtPtr consts;
    StmtPtr vars;
    StmtPtr types;
    std::vector<NodePtr> subprograms;
    StmtPtr mainBlock;

    ProgramNode(Token n,
                StmtPtr c,
                StmtPtr v,
                StmtPtr t,
                std::vector<NodePtr> subs,
                StmtPtr m)
      : name(std::move(n)), consts(std::move(c)), vars(std::move(v)),
        types(std::move(t)), subprograms(std::move(subs)),
        mainBlock(std::move(m)) {}
};

class ConstSectionNode : public StmtNode {
public:
    std::vector<std::pair<Token, ExprPtr>> entries;
    ConstSectionNode(std::vector<std::pair<Token, ExprPtr>> e)
      : entries(std::move(e)) {}
};

class VarSectionNode : public StmtNode {
public:
    std::vector<std::pair<Token, Token>> entries;
    VarSectionNode(std::vector<std::pair<Token, Token>> e)
      : entries(std::move(e)) {}
};

class TypeSectionNode : public StmtNode {
public:
    std::vector<std::pair<Token, Token>> entries;
    TypeSectionNode(std::vector<std::pair<Token, Token>> e)
      : entries(std::move(e)) {}
};

class FunctionDeclNode : public Node {
public:
    Token name;
    Token returnType;
    std::vector<std::pair<Token, Token>> params;
    StmtPtr body;

    FunctionDeclNode(Token n,
                     Token ret,
                     std::vector<std::pair<Token, Token>> p,
                     StmtPtr b)
      : name(std::move(n)), returnType(std::move(ret)),
        params(std::move(p)), body(std::move(b)) {}
};

class BlockNode : public StmtNode {
public:
    std::vector<StmtPtr> statements;
    BlockNode(std::vector<StmtPtr> stmts)
      : statements(std::move(stmts)) {}
};

class IfNode : public StmtNode {
public:
    ExprPtr cond;
    StmtPtr thenBr;
    StmtPtr elseBr;
    IfNode(ExprPtr c, StmtPtr t, StmtPtr e)
      : cond(std::move(c)), thenBr(std::move(t)), elseBr(std::move(e)) {}
};

class WhileNode : public StmtNode {
public:
    ExprPtr cond;
    StmtPtr body;
    WhileNode(ExprPtr c, StmtPtr b)
      : cond(std::move(c)), body(std::move(b)) {}
};

class ForNode : public StmtNode {
public:
    Token var;
    ExprPtr start;
    ExprPtr end;
    bool toUp;
    StmtPtr body;
    ForNode(Token v, ExprPtr s, ExprPtr e, bool u, StmtPtr b)
      : var(std::move(v)), start(std::move(s)), end(std::move(e)),
        toUp(u), body(std::move(b)) {}
};

class RepeatNode : public StmtNode {
public:
    std::vector<StmtPtr> body;
    ExprPtr cond;
    RepeatNode(std::vector<StmtPtr> b, ExprPtr c)
      : body(std::move(b)), cond(std::move(c)) {}
};

class CaseNode : public StmtNode {
public:
    ExprPtr expr;
    std::vector<std::pair<std::vector<ExprPtr>, StmtPtr>> branches;
    CaseNode(ExprPtr e,
             std::vector<std::pair<std::vector<ExprPtr>, StmtPtr>> b)
      : expr(std::move(e)), branches(std::move(b)) {}
};

class AssignNode : public StmtNode {
public:
    Token target;
    ExprPtr value;
    AssignNode(Token t, ExprPtr v)
      : target(std::move(t)), value(std::move(v)) {}
};

class ProcCallNode : public StmtNode {
public:
    Token name;
    std::vector<ExprPtr> args;
    ProcCallNode(Token n, std::vector<ExprPtr> a)
      : name(std::move(n)), args(std::move(a)) {}
};

// Expression nodes
class LiteralNode : public ExprNode {
public:
    Token tok;
    LiteralNode(Token t) : tok(std::move(t)) {}
};

class VarNode : public ExprNode {
public:
    Token tok;
    VarNode(Token t) : tok(std::move(t)) {}
};

class BinaryNode : public ExprNode {
public:
    Token op;
    ExprPtr left;
    ExprPtr right;
    BinaryNode(Token o, ExprPtr l, ExprPtr r)
      : op(std::move(o)), left(std::move(l)), right(std::move(r)) {}
};

// Parser class
class Parser {
public:
    Parser(const std::vector<Token>& toks) : tokens(toks), pos(0) {}
    std::unique_ptr<ProgramNode> parseProgram() { return program(); }

private:
    std::string TokenTypeToString(TokenType type) {
        switch (type) {
            case TokenType::PROGRAM: return "PROGRAM";
            case TokenType::VAR: return "VAR";
            case TokenType::CONST: return "CONST";
            case TokenType::PROCEDURE: return "PROCEDURE";
            case TokenType::FUNCTION: return "FUNCTION";
            case TokenType::LABEL: return "LABEL";
            case TokenType::BEGIN: return "BEGIN";
            case TokenType::DOWNTO: return "DOWNTO";
            case TokenType::TO: return "TO";
            case TokenType::END: return "END";
            case TokenType::IF: return "IF";
            case TokenType::THEN: return "THEN";
            case TokenType::ELSE: return "ELSE";
            case TokenType::RAISE: return "RAISE";
            case TokenType::CATCH: return "CATCH";
            case TokenType::TRY: return "TRY";
            case TokenType::FINALLY: return "FINALLY";
            case TokenType::RECORD: return "RECORD";
            case TokenType::REPEAT: return "REPEAT";
            case TokenType::TYPE: return "TYPE";
            case TokenType::UNTIL: return "UNTIL";
            case TokenType::USES: return "USES";
            case TokenType::WHILE: return "WHILE";
            case TokenType::FOR: return "FOR";
            case TokenType::DO: return "DO";
            case TokenType::OR: return "OR";
            case TokenType::IN: return "IN";
            case TokenType::AND: return "AND";
            case TokenType::NOT: return "NOT";
            case TokenType::DIV: return "DIV";
            case TokenType::IDENTIFIER: return "IDENTIFIER";
            case TokenType::INTEGER: return "INTEGER";
            case TokenType::REAL: return "REAL";
            case TokenType::BOOLEAN: return "BOOLEAN";
            case TokenType::STRING: return "STRING";
            case TokenType::INT_LIT: return "INT_LIT";
            case TokenType::REAL_LIT: return "REAL_LIT";
            case TokenType::BOOL_LIT: return "BOOL_LIT";
            case TokenType::STRING_LIT: return "STRING_LIT";
            case TokenType::ASSIGN: return "ASSIGN";
            case TokenType::PLUS: return "PLUS";
            case TokenType::MINUS: return "MINUS";
            case TokenType::MULTIPLY: return "MULTIPLY";
            case TokenType::DIVIDE: return "DIVIDE";
            case TokenType::LESS: return "LESS";
            case TokenType::GREATER: return "GREATER";
            case TokenType::LESS_EQUAL: return "LESS_EQUAL";
            case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
            case TokenType::EQUAL: return "EQUAL";
            case TokenType::NOT_EQUAL: return "NOT_EQUAL";
            case TokenType::OPEN_PAREN: return "OPEN_PAREN";
            case TokenType::CLOSE_PAREN: return "CLOSE_PAREN";
            case TokenType::OPEN_BRACK: return "OPEN_BRACK";
            case TokenType::CLOSE_BRACK: return "CLOSE_BRACK";
            case TokenType::DOT: return "DOT";
            case TokenType::COMMA: return "COMMA";
            case TokenType::SEMICOLON: return "SEMICOLON";
            case TokenType::COLON: return "COLON";
            default: return "UNKNOWN";
        }
    }

    const std::vector<Token>& tokens;
    size_t pos;

    const Token& peek() const { return tokens[pos]; }
    const Token& advance() { return tokens[pos++]; }
    bool match(TokenType type) {
        if (peek().type == type) { advance(); return true; }
        return false;
    }
    void expect(TokenType type, const std::string& msg) {
        if (!match(type)) throw std::runtime_error(msg);
    }

    // program: 'program' IDENT ';' constDecl varDecl typeDecl subprogram* block '.'
    std::unique_ptr<ProgramNode> program() {
        expect(TokenType::PROGRAM, "Esperado 'program'");
        Token name = advance();
        expect(TokenType::SEMICOLON, "Esperado ';'");
        auto consts = parseConstDecl();
        auto vars   = parseVarDecl();
        auto types  = parseTypeDecl();
        std::vector<NodePtr> subs;
        while (peek().type == TokenType::FUNCTION || peek().type == TokenType::PROCEDURE) {
            subs.push_back(parseSubprogram());
        }
        auto mainBlk = parseBlock();
        expect(TokenType::DOT, "Esperado '.' no fim");
        return std::make_unique<ProgramNode>(std::move(name),
                                             std::move(consts),
                                             std::move(vars),
                                             std::move(types),
                                             std::move(subs),
                                             std::move(mainBlk));
    }

    StmtPtr parseConstDecl() {
        std::vector<std::pair<Token, ExprPtr>> entries;
        if (match(TokenType::CONST)) {
            do {
                Token id = advance();
                expect(TokenType::EQUAL, "Esperado '='");
                auto val = parseExpression();
                expect(TokenType::SEMICOLON, "Esperado ';'");
                entries.emplace_back(std::move(id), std::move(val));
            } while (peek().type == TokenType::IDENTIFIER);
        }
        return std::make_unique<ConstSectionNode>(std::move(entries));
    }

    StmtPtr parseVarDecl() {
        std::vector<std::pair<Token, Token>> entries;
        if (match(TokenType::VAR)) {
            do {
                Token id = advance();
                std::cout << "Foi aqui o problema" << TokenTypeToString(peek().type) << std::endl;
                expect(TokenType::COLON, "Esperado ':'");
                Token typeTok = advance();
                expect(TokenType::SEMICOLON, "Esperado ';'");
                entries.emplace_back(std::move(id), std::move(typeTok));
            } while (peek().type == TokenType::IDENTIFIER);
        }
        return std::make_unique<VarSectionNode>(std::move(entries));
    }

    StmtPtr parseTypeDecl() {
        std::vector<std::pair<Token, Token>> entries;
        if (match(TokenType::TYPE)) {
            do {
                Token id = advance();
                expect(TokenType::EQUAL, "Esperado '='");
                Token def = advance();
                expect(TokenType::SEMICOLON, "Esperado ';'");
                entries.emplace_back(std::move(id), std::move(def));
            } while (peek().type == TokenType::IDENTIFIER);
        }
        return std::make_unique<TypeSectionNode>(std::move(entries));
    }

    NodePtr parseSubprogram() {
        Token kw = advance();
        Token name = advance();
        expect(TokenType::OPEN_PAREN, "Esperado '('");
        std::vector<std::pair<Token, Token>> params;
        if (peek().type != TokenType::CLOSE_PAREN) {
            do {
                Token pid = advance();
                expect(TokenType::COLON, "Esperado ':'");
                Token ptype = advance();
                params.emplace_back(std::move(pid), std::move(ptype));
            } while (match(TokenType::SEMICOLON));
        }
        expect(TokenType::CLOSE_PAREN, "Esperado ')'");
        Token retType;
        if (kw.type == TokenType::FUNCTION) {
            expect(TokenType::COLON, "Esperado ':' antes tipo");
            retType = advance();
        }
        expect(TokenType::SEMICOLON, "Esperado ';'");
        auto body = parseBlock();
        expect(TokenType::SEMICOLON, "Esperado ';' apos subprograma");
        return std::make_unique<FunctionDeclNode>(std::move(name), std::move(retType), std::move(params), std::move(body));
    }

    StmtPtr parseBlock() {
        expect(TokenType::BEGIN, "Esperado 'begin'");
        std::vector<StmtPtr> stmts;
        while (peek().type != TokenType::END) {
            stmts.push_back(parseStatement());
        }
        expect(TokenType::END, "Esperado 'end'");
        expect(TokenType::SEMICOLON, "Esperado ';'");
        return std::make_unique<BlockNode>(std::move(stmts));
    }

    StmtPtr parseStatement() {
        switch (peek().type) {
            case TokenType::IF: return parseIf();
            case TokenType::WHILE: return parseWhile();
            case TokenType::FOR: return parseFor();
            case TokenType::REPEAT: return parseRepeat();
            case TokenType::CASE: return parseCase();
            case TokenType::IDENTIFIER: {
                if (tokens[pos+1].type == TokenType::ASSIGN)
                    return parseAssignment();
                else
                    return parseProcedureCall();
            }
            default:
                throw std::runtime_error("Comando inesperado: " + TokenTypeToString(peek().type));
        }
    }

    StmtPtr parseIf() {
        advance();
        auto cond = parseExpression();
        expect(TokenType::THEN, "Esperado 'then'");
        auto thenBr = parseStatement();
        StmtPtr elseBr = nullptr;
        if (match(TokenType::ELSE)) {
            elseBr = parseStatement();
        }
        return std::make_unique<IfNode>(std::move(cond), std::move(thenBr), std::move(elseBr));
    }

    StmtPtr parseWhile() {
        advance();
        auto cond = parseExpression();
        expect(TokenType::DO, "Esperado 'do'");
        auto body = parseStatement();
        return std::make_unique<WhileNode>(std::move(cond), std::move(body));
    }

    StmtPtr parseFor() {
        advance();
        Token var = advance();
        expect(TokenType::ASSIGN, "Esperado ':='");
        auto start = parseExpression();
        bool toUp = match(TokenType::TO);
        if (!toUp) expect(TokenType::DOWNTO, "Esperado 'to' ou 'downto'");
        auto end = parseExpression();
        expect(TokenType::DO, "Esperado 'do'");
        auto body = parseStatement();
        return std::make_unique<ForNode>(std::move(var), std::move(start), std::move(end), toUp, std::move(body));
    }

    StmtPtr parseRepeat() {
        advance();
        std::vector<StmtPtr> body;
        while (peek().type != TokenType::UNTIL) {
            body.push_back(parseStatement());
        }
        advance();
        auto cond = parseExpression();
        expect(TokenType::SEMICOLON, "Esperado ';'");
        return std::make_unique<RepeatNode>(std::move(body), std::move(cond));
    }

    StmtPtr parseCase() {
        advance();
        auto expr = parseExpression();
        expect(TokenType::OF, "Esperado 'of'");
        std::vector<std::pair<std::vector<ExprPtr>, StmtPtr>> branches;
        do {
            std::vector<ExprPtr> labels;
            do {
                labels.push_back(parseExpression());
            } while (match(TokenType::COMMA));
            expect(TokenType::COLON, "Esperado ':'");
            auto stmt = parseStatement();
            expect(TokenType::SEMICOLON, "Esperado ';'");
            branches.emplace_back(std::move(labels), std::move(stmt));
        } while (peek().type == TokenType::REAL_LIT || peek().type == TokenType::INT_LIT|| peek().type == TokenType::STRING_LIT);
        expect(TokenType::END, "Esperado 'end' no case'");
        expect(TokenType::SEMICOLON, "Esperado ';'");
        return std::make_unique<CaseNode>(std::move(expr), std::move(branches));
    }

    StmtPtr parseAssignment() {
        Token id = advance();
        expect(TokenType::ASSIGN, "Esperado ':='");
        auto val = parseExpression();
        expect(TokenType::SEMICOLON, "Esperado ';'");
        return std::make_unique<AssignNode>(std::move(id), std::move(val));
    }

    StmtPtr parseProcedureCall() {
        Token name = advance();
        expect(TokenType::OPEN_PAREN, "Esperado '('");
        std::vector<ExprPtr> args;
        if (peek().type != TokenType::CLOSE_PAREN) {
            do {
                args.push_back(parseExpression());
            } while (match(TokenType::COMMA));
        }
        expect(TokenType::CLOSE_PAREN, "Esperado ')'");
        expect(TokenType::SEMICOLON, "Esperado ';'");
        return std::make_unique<ProcCallNode>(std::move(name), std::move(args));
    }

    // Expressions (precedence climbing)
    ExprPtr parseExpression() { return parseRelational(); }

    ExprPtr parseRelational() {
        auto left = parseAdditive();
        while (peek().type == TokenType::LESS || peek().type == TokenType::GREATER
            || peek().type == TokenType::EQUAL) {
            Token op = advance();
            auto right = parseAdditive();
            left = std::make_unique<BinaryNode>(std::move(op), std::move(left), std::move(right));
        }
        return left;
    }

    ExprPtr parseAdditive() {
        auto left = parseMultiplicative();
        while (peek().type == TokenType::PLUS || peek().type == TokenType::MINUS) {
            Token op = advance();
            auto right = parseMultiplicative();
            left = std::make_unique<BinaryNode>(std::move(op), std::move(left), std::move(right));
        }
        return left;
    }

    ExprPtr parseMultiplicative() {
        auto left = parseUnary();
        while (peek().type == TokenType::MULTIPLY || peek().type == TokenType::DIVIDE) {
            Token op = advance();
            auto right = parseUnary();
            left = std::make_unique<BinaryNode>(std::move(op), std::move(left), std::move(right));
        }
        return left;
    }

    ExprPtr parseUnary() {
        if (peek().type == TokenType::PLUS || peek().type == TokenType::MINUS) {
            Token op = advance();
            auto expr = parseUnary();
            // treat unary as binary with zero
            return std::make_unique<BinaryNode>(std::move(op), std::make_unique<LiteralNode>(Token{TokenType::INT_LIT, "0"}), std::move(expr));
        }
        return parsePrimary();
    }

    ExprPtr parsePrimary() {
        if (peek().type == TokenType::REAL_LIT || peek().type == TokenType::INT_LIT|| peek().type == TokenType::STRING_LIT) {
            return std::make_unique<LiteralNode>(advance());
        }
        if (peek().type == TokenType::IDENTIFIER) {
            return std::make_unique<VarNode>(advance());
        }
        if (match(TokenType::OPEN_PAREN)) {
            auto expr = parseExpression();
            expect(TokenType::CLOSE_PAREN, "Esperado ')'");
            return expr;
        }
        throw std::runtime_error("Expressão inesperada: " + TokenTypeToString(peek().type));
    }
};

#endif // PARSER_HPP

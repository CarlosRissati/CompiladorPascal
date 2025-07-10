#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include "tokenization.hpp"
#include "syntax_tree.hpp" // Assuming you have a syntax tree representation

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    void parse();

private:
    std::vector<Token> tokens;
    size_t current;

    // Methods for parsing different constructs
    void parseProgram();
    void parseVariableDeclaration();
    void parseProcedureDeclaration();
    void parseFunctionDeclaration();
    void parseStatement();
    void parseExpression();

    // Utility methods
    Token advance();
    Token peek() const;
    bool match(TokenType type);
    void consume(TokenType type, const std::string& errorMessage);
};

#endif // PARSER_HPP
#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "tokenization.hpp"
#include "ast.hpp"

class Parser {
public:
    explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {}

    void parse() {
        program();
    }

private:
    const std::vector<Token> m_tokens;
    size_t m_index = 0;

    Token current() const {
        if (m_index >= m_tokens.size()) {
            std::cerr << "Error: Unexpected end of token list." << std::endl;
            exit(EXIT_FAILURE);
        }
        return m_tokens[m_index];
    }

    Token advance() {
        if (m_index < m_tokens.size()) {
            return m_tokens[m_index++];
        }
        std::cerr << "Error: Attempting to advance beyond the end." << std::endl;
        return m_tokens.back();
    }

    bool match(TokenType type) {
        if (current().type == type) {
            advance();
            return true;
        }
        return false;
    }

    void expect(TokenType type, const std::string& msg) {
        if (!match(type)) {
            std::cerr << "Syntax error: " << msg << " (expected: " << TokenTypeToString(type) 
                      << ", found: " << TokenTypeToString(current().type) << ")" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void program() {
        expect(TokenType::PROGRAM, "Expected 'program'");
        expect(TokenType::IDENTIFIER, "Expected program identifier");
        expect(TokenType::SEMICOLON, "Expected ';' after program name");

        if (match(TokenType::VAR)) {
            declarations();
        }

        block();
        expect(TokenType::DOT, "Expected '.' at the end of the program");
    }

    void declarations() {
        do {
            expect(TokenType::IDENTIFIER, "Expected variable name");
            expect(TokenType::COLON, "Expected ':' after variable name");
            type();
            expect(TokenType::SEMICOLON, "Expected ';' after variable declaration");
        } while (current().type == TokenType::IDENTIFIER);
    }

    void type() {
        if (!(match(TokenType::INTEGER) || match(TokenType::REAL) || match(TokenType::BOOLEAN) || match(TokenType::STRING))) {
            std::cerr << "Error: Expected type (integer, real, boolean, or string)" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void block() {
        expect(TokenType::BEGIN, "Expected 'begin'");
        statements();
        expect(TokenType::END, "Expected 'end'");
    }

    void statements() {
        while (current().type != TokenType::END && current().type != TokenType::DOT) {
            statement();
            expect(TokenType::SEMICOLON, "Expected ';' after statement");
        }
    }

    void statement() {
        if (match(TokenType::IDENTIFIER)) {
            if (match(TokenType::ASSIGN)) {
                // Handle assignment
            } else {
                std::cerr << "Error: Invalid statement." << std::endl;
                exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "Error: Invalid statement." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    std::string TokenTypeToString(TokenType type) {
        switch (type) {
            case TokenType::PROGRAM: return "PROGRAM";
            case TokenType::VAR: return "VAR";
            case TokenType::IDENTIFIER: return "IDENTIFIER";
            case TokenType::INTEGER: return "INTEGER";
            case TokenType::REAL: return "REAL";
            case TokenType::BOOLEAN: return "BOOLEAN";
            case TokenType::STRING: return "STRING";
            case TokenType::ASSIGN: return "ASSIGN";
            case TokenType::SEMICOLON: return "SEMICOLON";
            case TokenType::COLON: return "COLON";
            case TokenType::BEGIN: return "BEGIN";
            case TokenType::END: return "END";
            case TokenType::DOT: return "DOT";
            default: return "UNKNOWN";
        }
    }
};
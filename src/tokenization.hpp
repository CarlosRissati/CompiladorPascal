#pragma once
#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include <unordered_map>
#include <cctype>

enum class Tipo_de_token {
    // Palavras reservadas
    PROGRAM, VAR, CONST, PROCEDURE, FUNCTION, LABEL, BEGIN, END,
    DOWNTO, TO, IF, THEN, ELSE, CASE, OF, EXCEPT, RAISE, CATCH,
    TRY, FINALLY, RECORD, REPEAT, TYPE, UNTIL, USES, WHILE, FOR, DO,
    OR, IN, AND, NOT, DIV,

    // Identificador e tipos
    IDENTIFIER, INTEGER, REAL, BOOLEAN, STRING,

    // Literias
    INT_LIT, REAL_LIT, BOOL_LIT, STRING_LIT,

    // Simbolos
    ASSIGN, PLUS, MINUS, MULTIPLY, DIVIDE, LESS, GREATER, LESS_EQUAL,
    GREATER_EQUAL, EQUAL, NOT_EQUAL, OPEN_PAREN, CLOSE_PAREN, OPEN_BRACK,
    CLOSE_BRACK, DOT, COMMA, SEMICOLON, COLON
};

struct Token {
    Tipo_de_token type;
    std::optional<std::string> value {};
    int line = 0;
    int col = 0;
};

class Tokenizer {
public:
    inline explicit Tokenizer(std::string input) : m_input(std::move(input)), m_index(0), m_line(1), m_col(1) {}

    inline std::vector<Token> tokenize();

private:
    [[nodiscard]] inline std::optional<char> peak(int ahead = 0) const {
        if (m_index + ahead >= m_input.length()) return {};
        return m_input.at(m_index + ahead);
    }
    
    inline char consume() {
        char currentChar = m_input.at(m_index++);
        if (currentChar == '\n') {
            m_line++;
            m_col = 1;
        } else {
            m_col++;
        }
        return currentChar;
    }

    const std::string m_input;
    size_t m_index;
    int m_line;
    int m_col;
};

inline std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;
    const std::unordered_map<std::string, Tipo_de_token> keywords = {
        {"program", Tipo_de_token::PROGRAM}, {"var", Tipo_de_token::VAR}, {"const", Tipo_de_token::CONST},
        {"procedure", Tipo_de_token::PROCEDURE}, {"function", Tipo_de_token::FUNCTION}, {"label", Tipo_de_token::LABEL},
        {"begin", Tipo_de_token::BEGIN}, {"end", Tipo_de_token::END}, {"if", Tipo_de_token::IF}, {"then", Tipo_de_token::THEN},
        {"else", Tipo_de_token::ELSE}, {"while", Tipo_de_token::WHILE}, {"do", Tipo_de_token::DO}, {"for", Tipo_de_token::FOR},
        {"to", Tipo_de_token::TO}, {"downto", Tipo_de_token::DOWNTO}, {"repeat", Tipo_de_token::REPEAT}, {"until", Tipo_de_token::UNTIL},
        {"integer", Tipo_de_token::INTEGER}, {"real", Tipo_de_token::REAL}, {"boolean", Tipo_de_token::BOOLEAN},
        {"string", Tipo_de_token::STRING}, {"true", Tipo_de_token::BOOL_LIT}, {"false", Tipo_de_token::BOOL_LIT},
        {"div", Tipo_de_token::DIV}, {"and", Tipo_de_token::AND}, {"or", Tipo_de_token::OR}, {"not", Tipo_de_token::NOT}
    };

    while (m_index < m_input.length()) {
        char current_char = m_input[m_index];
        int start_col = m_col;

        if (std::isspace(current_char)) { consume(); continue; }

        if (current_char == '{') {
            consume();
            while (peak().has_value() && peak().value() != '}') { consume(); }
            if(peak().has_value()) consume(); // Consome o '}'
            continue;
        }

        if (std::isalpha(current_char) || current_char == '_') {
            std::string buf; buf += consume();
            while (peak().has_value() && (std::isalnum(peak().value()) || peak().value() == '_')) { buf += consume(); }
            if (keywords.count(buf)) {
                tokens.push_back({keywords.at(buf), buf, m_line, start_col});
            } else {
                tokens.push_back({Tipo_de_token::IDENTIFIER, buf, m_line, start_col});
            }
            continue;
        }

        if (std::isdigit(current_char)) {
            std::string buf; buf += consume();
            bool is_real = false;
            while (peak().has_value() && (std::isdigit(peak().value()) || (peak().value() == '.' && !is_real))) {
                if (peak().value() == '.') is_real = true;
                buf += consume();
            }
            tokens.push_back({is_real ? Tipo_de_token::REAL_LIT : Tipo_de_token::INT_LIT, buf, m_line, start_col});
            continue;
        }

        if (current_char == '\'') {
            consume();
            std::string buf;
            while (peak().has_value() && peak().value() != '\'') { buf += consume(); }
            if (peak().has_value()) consume(); 
            else std::cerr << "Erro lexico: String nao terminada na linha " << m_line << std::endl;
            tokens.push_back({Tipo_de_token::STRING_LIT, buf, m_line, start_col});
            continue;
        }

        // Simbolos
        if (current_char == ':' && peak(1) == '=') { tokens.push_back({Tipo_de_token::ASSIGN, ":=", m_line, start_col}); consume(); consume(); }
        else if (current_char == '<' && peak(1) == '>') { tokens.push_back({Tipo_de_token::NOT_EQUAL, "<>", m_line, start_col}); consume(); consume(); }
        else if (current_char == '<' && peak(1) == '=') { tokens.push_back({Tipo_de_token::LESS_EQUAL, "<=", m_line, start_col}); consume(); consume(); }
        else if (current_char == '>' && peak(1) == '=') { tokens.push_back({Tipo_de_token::GREATER_EQUAL, ">=", m_line, start_col}); consume(); consume(); }
        else {
             switch(current_char) {
                case '+': tokens.push_back({Tipo_de_token::PLUS, "+", m_line, start_col}); consume(); break;
                case '-': tokens.push_back({Tipo_de_token::MINUS, "-", m_line, start_col}); consume(); break;
                case '*': tokens.push_back({Tipo_de_token::MULTIPLY, "*", m_line, start_col}); consume(); break;
                case '/': tokens.push_back({Tipo_de_token::DIVIDE, "/", m_line, start_col}); consume(); break;
                case '=': tokens.push_back({Tipo_de_token::EQUAL, "=", m_line, start_col}); consume(); break;
                case '<': tokens.push_back({Tipo_de_token::LESS, "<", m_line, start_col}); consume(); break;
                case '>': tokens.push_back({Tipo_de_token::GREATER, ">", m_line, start_col}); consume(); break;
                case '(': tokens.push_back({Tipo_de_token::OPEN_PAREN, "(", m_line, start_col}); consume(); break;
                case ')': tokens.push_back({Tipo_de_token::CLOSE_PAREN, ")", m_line, start_col}); consume(); break;
                case ';': tokens.push_back({Tipo_de_token::SEMICOLON, ";", m_line, start_col}); consume(); break;
                case ':': tokens.push_back({Tipo_de_token::COLON, ":", m_line, start_col}); consume(); break;
                case '.': tokens.push_back({Tipo_de_token::DOT, ".", m_line, start_col}); consume(); break;
                case ',': tokens.push_back({Tipo_de_token::COMMA, ",", m_line, start_col}); consume(); break;
                default: std::cerr << "Erro lexico: Caractere inesperado '" << current_char << "' na linha " << m_line << std::endl; consume(); break;
            }
        }
    }
    return tokens;
}
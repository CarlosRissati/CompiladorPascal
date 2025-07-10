#ifndef TOKENIZATION_HPP
#define TOKENIZATION_HPP

#include <string>
#include <vector>
#include <optional>

enum class TokenType {
    PROGRAM,
    VAR,
    CONST,
    PROCEDURE,
    FUNCTION,
    LABEL,
    BEGIN,
    END,
    IF,
    THEN,
    ELSE,
    RAISE,
    CATCH,
    TRY,
    FINALLY,
    RECORD,
    REPEAT,
    TYPE,
    UNTIL,
    USES,
    WHILE,
    FOR,
    DO,
    OR,
    IN,
    AND,
    NOT,
    DIV,
    IDENTIFIER,
    INTEGER,
    REAL,
    BOOLEAN,
    STRING,
    INT_LIT,
    REAL_LIT,
    BOOL_LIT,
    STRING_LIT,
    ASSIGN,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LESS,
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL,
    EQUAL,
    NOT_EQUAL,
    OPEN_PAREN,
    CLOSE_PAREN,
    OPEN_BRACK,
    CLOSE_BRACK,
    DOT,
    COMMA,
    SEMICOLON,
    COLON,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer {
public:
    explicit Tokenizer(std::string source);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t currentIndex;

    char advance();
    char peek() const;
    bool isAtEnd() const;
    void skipWhitespace();
    Token createToken(TokenType type, const std::optional<std::string>& value = std::nullopt);
    // Additional methods for tokenizing specific constructs
};

#endif // TOKENIZATION_HPP
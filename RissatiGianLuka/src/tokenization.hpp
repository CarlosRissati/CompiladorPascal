#pragma once //processa a header apenas uma vez
#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include <unordered_map>
#include <regex>

enum class TokenType {
    //PALAVRAS RESERVADAS
    PROGRAM,
    VAR,
    CONST,
    PROCEDURE,
    FUNCTION,
    LABEL,
    BEGIN,
    DOWNTO,
    TO,
    END,
    IF,
    THEN,
    ELSE,
    CASE,
    OF,
    EXCEPT,
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
    //identificador
    IDENTIFIER,
    //tipos
    INTEGER,
    REAL,
    BOOLEAN,
    STRING,
    INT_LIT,
    REAL_LIT,
    BOOL_LIT,
    STRING_LIT,
    //SIMBOLOS
    ASSIGN, //:=
    PLUS, //+
    MINUS, //-
    MULTIPLY, //*
    DIVIDE, // /
    LESS, // <
    GREATER, // >
    LESS_EQUAL, // <=
    GREATER_EQUAL, // >=
    EQUAL, // =
    NOT_EQUAL, // <>
    OPEN_PAREN, // (
    CLOSE_PAREN, // )
    OPEN_BRACK, // [
    CLOSE_BRACK, // ]
    DOT, // .
    COMMA, // ,
    SEMICOLON, // ;
    COLON // :
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};

class Tokenizer {
public:
    inline explicit Tokenizer(const std::string& input) : m_input(std::move(input)){
    } 

    inline std::vector<Token> tokenize(){
        std::vector<Token> tokens;

        std::string buf;

        const std::unordered_map<std::string, TokenType> palavrasChave = {
            {"program", TokenType::PROGRAM},
            {"var", TokenType::VAR},
            {"const", TokenType::CONST},
            {"procedure", TokenType::PROCEDURE},
            {"function", TokenType::FUNCTION},
            {"label", TokenType::LABEL},
            {"begin", TokenType::BEGIN},
            {"downto", TokenType::DOWNTO},
            {"downto", TokenType::TO},
            {"end", TokenType::END},
            {"if", TokenType::IF},
            {"then", TokenType::THEN},
            {"else", TokenType::ELSE},
            {"case", TokenType::CASE},
            {"of", TokenType::OF},
            {"except", TokenType::EXCEPT},
            {"raise", TokenType::RAISE},
            {"catch", TokenType::CATCH},
            {"try", TokenType::TRY},
            {"finally", TokenType::FINALLY},
            {"record", TokenType::RECORD},
            {"repeat", TokenType::REPEAT},
            {"type", TokenType::TYPE},
            {"until", TokenType::UNTIL},
            {"uses", TokenType::USES},
            {"while", TokenType::WHILE},
            {"for", TokenType::FOR},
            {"do", TokenType::DO},
            {"or", TokenType::OR},
            {"in", TokenType::IN},
            {"and", TokenType::AND},
            {"not", TokenType::NOT},
            {"div", TokenType::DIV},
            {"integer", TokenType::INTEGER},
            {"real", TokenType::REAL},
            {"boolean", TokenType::BOOLEAN},
            {"string", TokenType::STRING},
            {"true", TokenType::BOOL_LIT},
            {"false", TokenType::BOOL_LIT}
        };

        const std::unordered_map<std::string, TokenType> simbolos = {
            {":=", TokenType::ASSIGN},
            {"+", TokenType::PLUS},
            {"-", TokenType::MINUS},
            {"*", TokenType::MULTIPLY},
            {"/", TokenType::DIVIDE},
            {"<", TokenType::LESS},
            {">", TokenType::GREATER},
            {"<=", TokenType::LESS_EQUAL},
            {">=", TokenType::GREATER_EQUAL},
            {"=", TokenType::EQUAL},
            {"<>", TokenType::NOT_EQUAL},
            {"(", TokenType::OPEN_PAREN},
            {")", TokenType::CLOSE_PAREN},
            {"[", TokenType::OPEN_BRACK},
            {"]", TokenType::CLOSE_BRACK},
            {".", TokenType::DOT},
            {",", TokenType::COMMA},
            {";", TokenType::SEMICOLON},
            {":", TokenType::COLON}
        };

        const std::regex identifierRegex(R"([a-zA-Z_][a-zA-Z0-9_]*)");

        while(peak().has_value()){
            if(std::isalpha(peak().value())){
                buf.push_back(consume());
                while(peak().has_value() && std::isalnum(peak().value())){
                    buf.push_back(consume());
                }

                if (/*buf == "program"*/ palavrasChave.contains(buf)){
                    tokens.push_back({palavrasChave.at(buf), buf});
                }else if(std::regex_match(buf, identifierRegex)){
                    tokens.push_back({TokenType::IDENTIFIER, buf});
                }else {
                    std::cerr << "Erro: Token desconhecido" << std::endl;
                    buf.clear();
                    exit(EXIT_FAILURE);
                }
                buf.clear();
            }else if(std::isdigit(peak().value())){
                buf.push_back(consume());
                
                bool isReal = false;
                while(peak().has_value() && (std::isdigit(peak().value()) || peak().value() == '.')){
                    if(isReal && peak().value()=='.'){
                        std::cerr << "Erro: Numero Real invalido" << std::endl; 
                        exit(EXIT_FAILURE);
                    }
                    if(peak().value() == '.' && !isReal){
                        isReal = true;
                    }
                    
                    buf.push_back(consume());
                }
                
                if(peak().has_value() && std::isalpha(peak().value())){
                    std::cerr << "Erro: numero seguido de letra" << std::endl;
                    exit(EXIT_FAILURE);
                }

                if(isReal){
                    tokens.push_back({TokenType::REAL_LIT, buf});
                }else {
                    tokens.push_back({TokenType::INT_LIT, buf});
                }
                buf.clear();
            }else if(peak().value() == ';'){
                tokens.push_back({TokenType::SEMICOLON, ";"});
                consume();
            }else if(std::isspace(peak().value())){
                consume();
            }else if(std::ispunct(peak().value())){
                buf.push_back(consume());
                while(peak().has_value() && std::ispunct(peak().value()) && peak().value() != ';'){
                    buf.push_back(consume());
                    if(buf == "(*" || buf == "{"){
                        break;
                    }
                }
                
                if(buf == "(*"){
                    while(peak().has_value()){
                        if(peak().value() == '*' && peak(1).value() == ')'){
                            break;
                        }
                        consume();
                    }
                    consume();
                    consume();
                }else if(buf == "{"){
                    while(peak().has_value()){
                        if(peak().value() == '}')
                            break;
                        consume();
                    }
                    consume();
                }else 
                if(buf[0] == '\"' || buf[0] == '\''){
                    while(peak().has_value() && peak().value() != buf[0]){
                        buf.push_back(consume());
                    }
                    buf.push_back(consume());
                    tokens.push_back({TokenType::STRING_LIT, buf});
                }else if(simbolos.contains(buf)){
                    tokens.push_back({simbolos.at(buf), buf});
                }else{
                    std::cerr << "Erro: Simbolo desconhecido \"" << buf << "\"" << std::endl;
                    exit(EXIT_FAILURE);
                }
                buf.clear();
                 
            }else{
                std::cerr << "Erro: Caractere desconhecido \"" << peak().value() << "\" encontrado." << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        
        // if(!peak().has_value() && m_input.at(m_index - 1) != ';'){
        //     std::cerr << "Erro: Talvez você tenha esquecido de colocar um ponto e vírgula no final do comando?" << std::endl;
        //     exit(EXIT_FAILURE);
        // }

        m_index = 0; // Reset index after tokenization

        return tokens;
    }

private:

    [[nodiscard]] inline std::optional<char> peak(int ahead = 0) const{
        if (m_index + (ahead == 0 ? 1 : ahead) > m_input.length()){
            return {};
        }else {
            return m_input.at(m_index + ahead);
        }
    }

    inline char consume() {
        return m_input.at(m_index++);
    }

    const std::string m_input;
    int m_index = 0;
};

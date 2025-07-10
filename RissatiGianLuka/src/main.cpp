#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

#include "tokenization.hpp"
#include "parser.hpp"

inline std::ostream& operator<<(std::ostream& os, TokenType type) {
    switch (type) {
        case TokenType::PROGRAM: return os << "PROGRAM";
        case TokenType::VAR: return os << "VAR";
        case TokenType::CONST: return os << "CONST";
        case TokenType::PROCEDURE: return os << "PROCEDURE";
        case TokenType::FUNCTION: return os << "FUNCTION";
        case TokenType::LABEL: return os << "LABEL";
        case TokenType::BEGIN: return os << "BEGIN";
        case TokenType::DOWNTO: return os << "DOWNTO";
        case TokenType::END: return os << "END";
        case TokenType::IF: return os << "IF";
        case TokenType::THEN: return os << "THEN";
        case TokenType::ELSE: return os << "ELSE";
        case TokenType::RAISE: return os << "RAISE";
        case TokenType::CATCH: return os << "CATCH";
        case TokenType::TRY: return os << "TRY";
        case TokenType::FINALLY: return os << "FINALLY";
        case TokenType::RECORD: return os << "RECORD";
        case TokenType::REPEAT: return os << "REPEAT";
        case TokenType::TYPE: return os << "TYPE";
        case TokenType::UNTIL: return os << "UNTIL";
        case TokenType::USES: return os << "USES";
        case TokenType::WHILE: return os << "WHILE";
        case TokenType::FOR: return os << "FOR";
        case TokenType::DO: return os << "DO";
        case TokenType::OR: return os << "OR";
        case TokenType::IN: return os << "IN";
        case TokenType::AND: return os << "AND";
        case TokenType::NOT: return os << "NOT";
        case TokenType::DIV: return os << "DIV";
        case TokenType::IDENTIFIER: return os << "IDENTIFIER";
        case TokenType::INTEGER: return os << "INTEGER";
        case TokenType::REAL: return os << "REAL";
        case TokenType::BOOLEAN: return os << "BOOLEAN";
        case TokenType::STRING: return os << "STRING";
        case TokenType::INT_LIT: return os << "INT_LIT";
        case TokenType::REAL_LIT: return os << "REAL_LIT";
        case TokenType::BOOL_LIT: return os << "BOOL_LIT";
        case TokenType::STRING_LIT: return os << "STRING_LIT";
        case TokenType::ASSIGN: return os << "ASSIGN";
        case TokenType::PLUS: return os << "PLUS";
        case TokenType::MINUS: return os << "MINUS";
        case TokenType::MULTIPLY: return os << "MULTIPLY";
        case TokenType::DIVIDE: return os << "DIVIDE";
        case TokenType::LESS: return os << "LESS";
        case TokenType::GREATER: return os << "GREATER";
        case TokenType::LESS_EQUAL: return os << "LESS_EQUAL";
        case TokenType::GREATER_EQUAL: return os << "GREATER_EQUAL";
        case TokenType::EQUAL: return os << "EQUAL";
        case TokenType::NOT_EQUAL: return os << "NOT_EQUAL";
        case TokenType::OPEN_PAREN: return os << "OPEN_PAREN";
        case TokenType::CLOSE_PAREN: return os << "CLOSE_PAREN";
        case TokenType::OPEN_BRACK: return os << "OPEN_BRACK";
        case TokenType::CLOSE_BRACK: return os << "CLOSE_BRACK";
        case TokenType::DOT: return os << "DOT";
        case TokenType::COMMA: return os << "COMMA";
        case TokenType::SEMICOLON: return os << "SEMICOLON";
        case TokenType::COLON: return os << "COLON";
        default: return os << "UNKNOWN";
    }
}

int main(int argc, char *argv[]){

    if(argc != 2){
        std::cerr << "Uso incorreto do comando" << std::endl;
        std::cerr << "Tente usar: compiler <arquivo a ser compilado>" << std::endl;
        return EXIT_FAILURE;
    }
        
    std::string conteudo;{
        std::stringstream conteudoStream;
        std::ifstream arquivo(argv[1], std::ios::in);;
        conteudoStream << arquivo.rdbuf();
        conteudo = conteudoStream.str();
    }

    std::cout << "Analisador léxico iniciado" << std::endl;
    Tokenizer tokenizer(std::move(conteudo));
    std::vector<Token> lista_tokens = tokenizer.tokenize();
    std::cout << "Analisador léxico finalizado" << std::endl;
    
    // for(Token &token : lista_tokens){
    //     std::cout << token.type << " -> " << token.value.value() << std::endl;
    // }
    std::cout << "Analisador sintatico iniciado" << std::endl;
    Parser parser(std::move(lista_tokens));
    parser.parse();
    std::cout << "Analisador sintatico finalizado" << std::endl;
    // std::optional<NodeExit> tree = parser.parse();

    return EXIT_SUCCESS;
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

#include "tokenization.hpp"
#include "parser.hpp"
#include "analisador_semantico.hpp"

inline std::ostream& operator<<(std::ostream& os, Tipo_de_token type) {
    switch (type) {
        case Tipo_de_token::PROGRAM: return os << "PROGRAM";
        case Tipo_de_token::VAR: return os << "VAR";
        case Tipo_de_token::CONST: return os << "CONST";
        case Tipo_de_token::PROCEDURE: return os << "PROCEDURE";
        case Tipo_de_token::FUNCTION: return os << "FUNCTION";
        case Tipo_de_token::LABEL: return os << "LABEL";
        case Tipo_de_token::BEGIN: return os << "BEGIN";
        case Tipo_de_token::DOWNTO: return os << "DOWNTO";
        case Tipo_de_token::END: return os << "END";
        case Tipo_de_token::IF: return os << "IF";
        case Tipo_de_token::THEN: return os << "THEN";
        case Tipo_de_token::ELSE: return os << "ELSE";
        case Tipo_de_token::RAISE: return os << "RAISE";
        case Tipo_de_token::CATCH: return os << "CATCH";
        case Tipo_de_token::TRY: return os << "TRY";
        case Tipo_de_token::FINALLY: return os << "FINALLY";
        case Tipo_de_token::RECORD: return os << "RECORD";
        case Tipo_de_token::REPEAT: return os << "REPEAT";
        case Tipo_de_token::TYPE: return os << "TYPE";
        case Tipo_de_token::UNTIL: return os << "UNTIL";
        case Tipo_de_token::USES: return os << "USES";
        case Tipo_de_token::WHILE: return os << "WHILE";
        case Tipo_de_token::FOR: return os << "FOR";
        case Tipo_de_token::DO: return os << "DO";
        case Tipo_de_token::OR: return os << "OR";
        case Tipo_de_token::IN: return os << "IN";
        case Tipo_de_token::AND: return os << "AND";
        case Tipo_de_token::NOT: return os << "NOT";
        case Tipo_de_token::DIV: return os << "DIV";
        case Tipo_de_token::IDENTIFIER: return os << "IDENTIFIER";
        case Tipo_de_token::INTEGER: return os << "INTEGER";
        case Tipo_de_token::REAL: return os << "REAL";
        case Tipo_de_token::BOOLEAN: return os << "BOOLEAN";
        case Tipo_de_token::STRING: return os << "STRING";
        case Tipo_de_token::INT_LIT: return os << "INT_LIT";
        case Tipo_de_token::REAL_LIT: return os << "REAL_LIT";
        case Tipo_de_token::BOOL_LIT: return os << "BOOL_LIT";
        case Tipo_de_token::STRING_LIT: return os << "STRING_LIT";
        case Tipo_de_token::ASSIGN: return os << "ASSIGN";
        case Tipo_de_token::PLUS: return os << "PLUS";
        case Tipo_de_token::MINUS: return os << "MINUS";
        case Tipo_de_token::MULTIPLY: return os << "MULTIPLY";
        case Tipo_de_token::DIVIDE: return os << "DIVIDE";
        case Tipo_de_token::LESS: return os << "LESS";
        case Tipo_de_token::GREATER: return os << "GREATER";
        case Tipo_de_token::LESS_EQUAL: return os << "LESS_EQUAL";
        case Tipo_de_token::GREATER_EQUAL: return os << "GREATER_EQUAL";
        case Tipo_de_token::EQUAL: return os << "EQUAL";
        case Tipo_de_token::NOT_EQUAL: return os << "NOT_EQUAL";
        case Tipo_de_token::OPEN_PAREN: return os << "OPEN_PAREN";
        case Tipo_de_token::CLOSE_PAREN: return os << "CLOSE_PAREN";
        case Tipo_de_token::OPEN_BRACK: return os << "OPEN_BRACK";
        case Tipo_de_token::CLOSE_BRACK: return os << "CLOSE_BRACK";
        case Tipo_de_token::DOT: return os << "DOT";
        case Tipo_de_token::COMMA: return os << "COMMA";
        case Tipo_de_token::SEMICOLON: return os << "SEMICOLON";
        case Tipo_de_token::COLON: return os << "COLON";
        default: return os << "UNKNOWN";
    }
}

int main(int argc, char *argv[]){

    if(argc != 2){
        std::cerr << "Uso incorreto. Correto: ./compiler <arquivo_de_codigo.pas>" << std::endl;
        return EXIT_FAILURE;
    }
        
    std::string conteudo;
    {
        std::ifstream arquivo(argv[1]);
        if (!arquivo.is_open()) {
            std::cerr << "Erro: Nao foi possivel abrir o arquivo " << argv[1] << std::endl;
            return EXIT_FAILURE;
        }
        std::stringstream conteudoStream;
        conteudoStream << arquivo.rdbuf();
        conteudo = conteudoStream.str();
    }

    try {
        std::cout << "Analise Lexica Iniciada..." << std::endl;
        Tokenizer tokenizer(std::move(conteudo));
        std::vector<Token> lista_tokens = tokenizer.tokenize();
        std::cout << "Analise Lexica Finalizada." << std::endl;
        
        std::cout << "Analise Sintatica Iniciada..." << std::endl;
        Parser parser(lista_tokens);
        NodePtr ast = parser.parseProgram();
        std::cout << "Analise Sintatica Finalizada." << std::endl;

        std::cout << "Analise Semantica Iniciada..." << std::endl;
        SemanticAnalyzer analyzer;
        analyzer.analyze(ast);
        std::cout << "Analise Semantica Finalizada." << std::endl;

        std::cout << "\nCompilacao finalizada com sucesso!" << std::endl;

    } catch (const std::runtime_error& e) {
        std::cerr << "Erro fatal durante a compilacao: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
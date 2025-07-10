#pragma once
#include <vector>
#include <iostream>

#include "tokenization.hpp"

struct NodeExpr {
    Token int_lit;
};

struct NodeExit {
    NodeExpr expr;
};

class Parser {
public:
    explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)){}

    void parse(){
        program();
    }

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

    const std::vector<Token> m_tokens;
    size_t m_index = 0;

    Token current() const {
        if (m_index >= m_tokens.size()){
            std::cerr << "Erro: Fim inesperado da lista de tokens." << std::endl;
            exit(EXIT_FAILURE);
        }
        return m_tokens[m_index];
    }

    Token advance() {
        if (m_index < m_tokens.size()){
            return m_tokens[m_index++];
        }
        std::cerr << "Erro: tentativa de avançar além do fim." << std::endl;
        return m_tokens.back();
    }

    bool match(TokenType type){
        if(current().type == type){
            advance();
            return true;
        }
        return false;
    }

    Token lookahead(int offset = 1) const{
        if(m_index + offset >= m_tokens.size()){
            return m_tokens.back();
        }
        return m_tokens[m_index + offset];
    }

    void expect(TokenType type, const std::string& msg){
        if(!match(type)){
            std::cerr << "Erro sintático: " << msg << " (esperado: " << TokenTypeToString(type) << ", encontrado: " << TokenTypeToString(current().type)<< ")" << std::endl;
            exit(EXIT_FAILURE);
        }
    }


    //parsing
    void program(){
        // std::cout << "DEBUG: current antes de program= " << TokenTypeToString(current().type) << "\n";
        expect(TokenType::PROGRAM, "Esperado 'program'");
        // std::cout << "DEBUG: current antes de identifier= " << TokenTypeToString(current().type) << "\n";
        expect(TokenType::IDENTIFIER, "Esperado identificador do programa");
        // std::cout << "DEBUG: current antes de semicolon= " << TokenTypeToString(current().type) << "\n";
        expect(TokenType::SEMICOLON, "Esperado ';' após o nome do programa");
        // std::cout << "DEBUG: current antes de label= " << TokenTypeToString(current().type) << "\n";

        // declaracoes2();

        if (match(TokenType::LABEL)) declaracoes_label();
        // std::cout << "DEBUG: current antes de const= " << TokenTypeToString(current().type) << "\n";
        if (match(TokenType::CONST)) declaracoes_const();
        // std::cout << "DEBUG: current antes de type= " << TokenTypeToString(current().type) << "\n";
        if (match(TokenType::TYPE)) declaracoes_type();
        // std::cout << "DEBUG: current antes de var= " << TokenTypeToString(current().type) << "\n";
        if (match(TokenType::VAR)) declaracoes_var();
        // std::cout << "DEBUG: current antes de procudure= " << TokenTypeToString(current().type) << "\n";
        
        declaracoes_func_procedure(); // procedures e functions

        // std::cout << "DEBUG: current = antes de bloco" << TokenTypeToString(current().type) << "\n";
        bloco();
        expect(TokenType::DOT, "Esperado '.' ao final do programa");
    }

    void declaracoes_var(){
        // expect(TokenType::VAR, "Esperado 'var'");
        // lista_var();
        // expect(TokenType::COLON, "Esperado ':' após os nomes declarados");
        // tipo();
        // expect(TokenType::SEMICOLON, "Esperado ';' após a declaração de tipos");

        // expect(TokenType::VAR, "Esperado 'var'");
    
        do {
            lista_var();
            expect(TokenType::COLON, "Esperado ':' após os nomes declarados");
            tipo();
            expect(TokenType::SEMICOLON, "Esperado ';' após a declaração de tipos");
        } while (current().type == TokenType::IDENTIFIER);
    }

    void lista_var(){
        expect(TokenType::IDENTIFIER, "Esperado nome de variavel");
        while(match(TokenType::COMMA)){
            expect(TokenType::IDENTIFIER, "Esperado nome de vaiável após ','");
        }
    }

    void tipo(){
        if(!(match(TokenType::INTEGER) || match(TokenType::REAL) || match(TokenType::BOOLEAN) || match(TokenType::STRING))){
            std::cerr << "Erro: tipo esperado (integer, real, boolean ou string)" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void bloco(){
        expect(TokenType::BEGIN, "Esperado incialização de bloco(begin)");
        lista_comandos();
        expect(TokenType::END, "Esperado finalização de bloco(end)");
    }

    void lista_comandos(){
        while(current().type != TokenType::END && current().type != TokenType::DOT){
            comando();
            expect(TokenType::SEMICOLON, "Esperado ';' após comando");
        }
    }

    void comando(){
        if(match(TokenType::IF)){
            comando_if();
        }else if(match(TokenType::WHILE)){
            comando_while();
        }else if(match(TokenType::REPEAT)){
            comando_repeat();
        }else if(match(TokenType::FOR)){
            comando_for();
        }else if(lookahead().type == TokenType::OPEN_PAREN){
            chamada_funcao_argumentos();
        }else if(lookahead().type == TokenType::ASSIGN){
            atribuicao();
        }else if(match(TokenType::BEGIN)){
            comando_composto    ();
        }else if (match(TokenType::TRY)) {
            comando_try();
        }else if (match(TokenType::CASE)) {
            comando_case();
        } else if (current().type == TokenType::IDENTIFIER) {
            if (lookahead().type == TokenType::OPEN_PAREN) {
                match(TokenType::IDENTIFIER);
                match(TokenType::OPEN_PAREN);
                chamada_funcao_argumentos();
            } else if (lookahead().type == TokenType::ASSIGN) {
                atribuicao(); // consome o IDENTIFIER
            } else {
                std::cerr << "Erro: comando iniciado por identificador inválido\n";
                exit(EXIT_FAILURE);
            }
        }else {
            std::cerr << "Erro: comando inválido." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void chamada_funcao_argumentos(){
        // expect(TokenType::IDENTIFIER, "Esperado nome da função");
        // expect(TokenType::OPEN_PAREN, "Esperado '('");
        // expressao();
        if (current().type != TokenType::CLOSE_PAREN) {
            expressao();
            while (match(TokenType::COMMA)) {
                expressao();
            }
        }
        expect(TokenType::CLOSE_PAREN, "Esperado ')'");
    }

    void atribuicao(){
        expect(TokenType::IDENTIFIER, "Esperado identificador");
        expect(TokenType::ASSIGN, "Esperado ':='"); //provavelmente n necessario por estar chamando atribuicao que um token de assign é encontrado
        expressao();
    }

    void expressao(){
        termo();
        while(match(TokenType::PLUS) || match(TokenType::MINUS)){
            termo();
        }
    }

    void termo(){
        fator();
        while(match(TokenType::MULTIPLY) || match(TokenType::DIVIDE) || match(TokenType::DIV)){
            fator();
        }
    }

    void fator(){
        if(match(TokenType::NOT)){
            fator();
        }else if(match(TokenType::OPEN_PAREN)){
            expressao();
            expect(TokenType::CLOSE_PAREN, "Esperado ')'");
        }else if(match(TokenType::IDENTIFIER)){
            if(match(TokenType::OPEN_PAREN)){
                chamada_funcao_argumentos();
            }
        } else if(match(TokenType::INT_LIT) || match(TokenType::REAL_LIT)){
            return;
        } else {
            std::cerr << "Erro: fator invalido";
            exit(EXIT_FAILURE); 
        }
    }

    void expr_relacional(){
        expressao();
        if(match(TokenType::EQUAL) || match(TokenType::LESS) || match(TokenType::GREATER) || match(TokenType::LESS_EQUAL) || match(TokenType::GREATER_EQUAL) || match(TokenType::NOT_EQUAL)){
            expressao();
        }
    }

    void comando_composto(){
        // expect(TokenType::BEGIN, "Esperado 'begin'");
        lista_comandos();
        expect(TokenType::END, "Esperado 'end'");
    }

    void comando_if(){
        // expect(TokenType::IF, "Esperado 'if'");
        expr_relacional();
        expect(TokenType::THEN, "Esperado 'then'");
        comando();
        if (match(TokenType::ELSE)){
            comando();
        }
    }

    void comando_while(){
        // expect(TokenType::WHILE, "Esperado 'while'");
        expr_relacional();
        expect(TokenType::DO, "Esperado 'do'");
        comando();
    }

    void comando_repeat(){
        // expect(TokenType::REPEAT, "Esperado 'repeat'");
        do {
            comando();
            expect(TokenType::SEMICOLON, "Esperado ';' entre comandos");
        }while(current().type != TokenType::UNTIL);
        expect(TokenType::UNTIL, "Esperado 'until'");
        expr_relacional();
    }

    void comando_for(){
        // expect(TokenType::FOR, "Esperado 'for'");
        expect(TokenType::IDENTIFIER, "Esperado variavel de controle");
        expect(TokenType::ASSIGN, "Esperado ':='");
        expressao();
        if(!(match(TokenType::TO) || match(TokenType::DOWNTO))){
            std::cerr << "Esperado 'to' ou 'downto'" << std::endl;
            exit(EXIT_FAILURE);
        }
        expressao();
        expect(TokenType::DO, "Esperado 'do'");
        comando();
    }

    void declaracao_procedure(){
        // expect(TokenType::PROCEDURE, "Esperado 'procedure'");
        expect(TokenType::IDENTIFIER, "Esperado nome da procedure");
        if(match(TokenType::OPEN_PAREN)){
            lista_parametros();
            expect(TokenType::CLOSE_PAREN, "Esperado ')'");
        }
        expect(TokenType::SEMICOLON, "Esperado ';'");
        bloco();
        expect(TokenType::SEMICOLON, "Esperado ';' após bloco de procedure");
    }

    void declaracao_function() {
        // expect(TokenType::FUNCTION, "Esperado 'function'");
        expect(TokenType::IDENTIFIER, "Esperado nome da função");
        if (match(TokenType::OPEN_PAREN)) {
            lista_parametros();
            expect(TokenType::CLOSE_PAREN, "Esperado ')'");
        }
        expect(TokenType::COLON, "Esperado ':' após nome da função");
        tipo();
        expect(TokenType::SEMICOLON, "Esperado ';'");
        bloco();
        expect(TokenType::SEMICOLON, "Esperado ';' após bloco da função");
    }

    void lista_parametros() {
        lista_var();
        expect(TokenType::COLON, "Esperado ':' após parâmetros");
        tipo();
        while (match(TokenType::SEMICOLON)) {
            lista_var();
            expect(TokenType::COLON, "Esperado ':' após parâmetros");
            tipo();
        }
    }

    // void declaracao_label() {
    //     expect(TokenType::LABEL, "Esperado 'label'");
    //     expect(TokenType::INT_LIT, "Esperado número de label");
    //     while (match(TokenType::COMMA)) {
    //         expect(TokenType::INT_LIT, "Esperado número de label");
    //     }
    //     expect(TokenType::SEMICOLON, "Esperado ';'");
    // }

    void declaracoes_type() {
        // expect(TokenType::TYPE, "Esperado 'type'");

        do {
            expect(TokenType::IDENTIFIER, "Esperado nome do novo tipo");
            expect(TokenType::EQUAL, "Esperado '='");

            if (match(TokenType::RECORD)) {
                do {
                    expect(TokenType::IDENTIFIER, "Esperado campo do record");
                    expect(TokenType::COLON, "Esperado ':'");
                    tipo();
                    expect(TokenType::SEMICOLON, "Esperado ';'");
                } while (current().type == TokenType::IDENTIFIER);

                expect(TokenType::END, "Esperado 'end' no final do record");
            } else {
                tipo(); // inteiro, real, boolean, string, etc.
            }

            expect(TokenType::SEMICOLON, "Esperado ';' após declaração de tipo");
        } while (current().type == TokenType::IDENTIFIER);

        // expect(TokenType::TYPE, "Esperado 'type'");
        // expect(TokenType::IDENTIFIER, "Esperado nome do tipo");
        // expect(TokenType::EQUAL, "Esperado '='");

        // if (match(TokenType::RECORD)) {
        //     do {
        //         expect(TokenType::IDENTIFIER, "Esperado campo do record");
        //         expect(TokenType::COLON, "Esperado ':'");
        //         tipo();
        //         expect(TokenType::SEMICOLON, "Esperado ';'");
        //     } while (current().type == TokenType::IDENTIFIER);
        //     expect(TokenType::END, "Esperado 'end' do record");
        // } else {
        //     tipo(); // outro tipo básico
        // }

        // expect(TokenType::SEMICOLON, "Esperado ';'");
    }

    void comando_try() {
        // expect(TokenType::TRY, "Esperado 'try'");
        lista_comandos();

        if (match(TokenType::CATCH) || match(TokenType::EXCEPT)) {
            lista_comandos();
        }

        if (match(TokenType::FINALLY)) {
            lista_comandos();
        }

        expect(TokenType::END, "Esperado 'end' ao final do try/catch");
    }

    void comando_case() {
        expect(TokenType::CASE, "Esperado 'case'");
        expressao();
        expect(TokenType::OF, "Esperado 'of'");

        do {
            expect(TokenType::INT_LIT, "Esperado valor do case");
            expect(TokenType::COLON, "Esperado ':'");
            comando();
            expect(TokenType::SEMICOLON, "Esperado ';'");
        } while (current().type == TokenType::INT_LIT);

        if (match(TokenType::ELSE)) {
            comando();
            expect(TokenType::SEMICOLON, "Esperado ';'");
        }

        expect(TokenType::END, "Esperado 'end' ao final do case");
    }

    void declaracoes_func_procedure() {
        while (true) {
            if (match(TokenType::PROCEDURE)) {
                declaracao_procedure();
            } else if (match(TokenType::FUNCTION)) {
                declaracao_function();
            } else {
                break;
            }
        }
    }

    void declaracoes_label() {
        // expect(TokenType::LABEL, "Esperado 'label'");
        expect(TokenType::INT_LIT, "Esperado número de label");

        while (match(TokenType::COMMA)) {
            expect(TokenType::INT_LIT, "Esperado número de label");
        }

        expect(TokenType::SEMICOLON, "Esperado ';' após declaração de label");
    }

    void declaracoes_const() {
        // expect(TokenType::CONST, "Esperado 'const'");

        do {
            expect(TokenType::IDENTIFIER, "Esperado identificador de constante");
            expect(TokenType::EQUAL, "Esperado '='");
            if (!match(TokenType::INT_LIT) && !match(TokenType::REAL_LIT) && !match(TokenType::BOOL_LIT) && !match(TokenType::STRING_LIT)) {
                std::cerr << "Erro: valor literal inválido em const\n";
                exit(EXIT_FAILURE);
            }
            expect(TokenType::SEMICOLON, "Esperado ';'");
        } while (current().type == TokenType::IDENTIFIER);
    }

};
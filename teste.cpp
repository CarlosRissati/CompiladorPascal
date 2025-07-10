#include <iostream>
#include <optional>

class Teste {
    public:
        Teste(){}

        int testador(){
            if(peak().has_value()) {
                std::cout << "O caractere é: " << peak().value() << std::endl;
            } else {
                std::cout << "O caractere não está definido." << std::endl;
            }
        }
    private:
        std::optional<char> peak() const {
            return {};
        }
};

int main() {
// #if __cplusplus == 202002L
//     std::cout << "C++20\n";
// #elif __cplusplus == 201703L
//     std::cout << "C++17\n";
// #elif __cplusplus == 201402L
//     std::cout << "C++14\n";
// #elif __cplusplus == 201103L
//     std::cout << "C++11\n";
// #elif __cplusplus == 199711L
//     std::cout << "C++98\n";
// #else
//     std::cout << "Versão desconhecida: " << __cplusplus << "\n";
// #endif

    std::string buf = "2";
    if(char c = (buf[0] == '\'' || buf[0] == '\"')){
        std::cout << c << std::endl;
    }

    // Teste teste;
    // teste.testador();
}

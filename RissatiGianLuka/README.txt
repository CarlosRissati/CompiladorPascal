Analisador Semantico


Alunos:
- Carlos Eduardo Rodrigues Simoes Rissati
- Gian Roberto Pereira da Silva
- Luka Alves Claro


Instrucoes para Compilacao e Execucao

O projeto utiliza CMake para a compilacao e foi escrito em C++17 padrão
garantindo compatibilidade com Linux e Windows.

Requisitos:
- C++ Compiler (GCC no Linux, ou MinGW/MSVC no Windows)
- CMake (versao 3.10 ou superior)

Passo a Passo para Compilar:

1 - Abra um terminal (ou PowerShell no Windows) na pasta raiz do projeto.

2 - Crie um diretorio para a compilacao (build):
   mkdir build
   cd build

3 - Execute o CMake para configurar o projeto. Ele detectara' o compilador automaticamente:
   cmake ..

4 - Execute o comando de build. Este comando e' universal e funciona em qualquer sistema:
   cmake --build .

   (Isso ira' compilar o codigo e criar o executavel 'compiler' ou 'compiler.exe' dentro da pasta 'build').


Passo a Passo para Executar:

Apos a compilacao, o executavel estara' dentro da pasta 'build'.

- No Linux:
  Estando dentro da pasta 'build', execute o seguinte comando, passando o arquivo de codigo como argumento:
  
  ./compiler ../caminho/para/seu_arquivo.pas

- No Windows:
  Estando dentro da pasta 'build', o comando e' ligeiramente diferente (barra invertida e .exe):
  
  .\compiler.exe ..\caminho\para\seu_arquivo.pas

Exemplo de uso com um arquivo 'teste.pas' na pasta principal do projeto:

# No Linux (dentro da pasta 'build'):
./compiler ../teste.pas

# No Windows (dentro da pasta 'build'):
.\compiler.exe ..\teste.pas
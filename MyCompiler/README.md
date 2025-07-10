# MyCompiler

MyCompiler is a simplified Pascal language compiler designed for educational purposes. This project includes a lexical analyzer, a parser, and a semantic analyzer to process Pascal source code.

## Project Structure

```
MyCompiler
├── src
│   ├── main.cpp               # Entry point of the compiler
│   ├── tokenization.hpp       # Header for the Tokenizer class
│   ├── parser.hpp             # Header for the Parser class
│   ├── semantic_analyzer.hpp   # Header for the SemanticAnalyzer class
│   ├── semantic_analyzer.cpp   # Implementation of the SemanticAnalyzer class
├── CMakeLists.txt             # CMake configuration file
└── README.md                   # Project documentation
```

## Features

- **Lexical Analysis**: Breaks down the source code into tokens for further processing.
- **Syntactic Analysis**: Parses the token stream to create a syntax tree.
- **Semantic Analysis**: Checks for semantic errors such as type mismatches and undeclared variables.

## Building the Project

To build the project, ensure you have CMake installed. Then, navigate to the project directory and run the following commands:

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

After building the project, you can run the compiler with the following command:

```bash
./MyCompiler <source_file.pas>
```

Replace `<source_file.pas>` with the path to your Pascal source file.

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue for any enhancements or bug fixes.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.
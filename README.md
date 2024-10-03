# Zara Compiler Construction Project

### Overview
This project is part of a learning exercise in **Compiler Construction**. The goal is to build a compiler for the **Zara programming language** by progressively implementing all phases of a compiler, from **Lexical Analysis** to **Code Generation**. Zara is a new language with data types such as `int`, `float`, `string`, `array`, and `stack`. It also supports control structures (`if-else`, `do-while`, `for` loops), operators, methods, and classes.

### Learning Goals
By the end of this project, we aim to gain a deeper understanding of:
- **Analysis Phases**: Lexical, Syntax, and Semantic Analysis.
- **Synthesis Phases**: Intermediate Code Generation, Optimization, and Code Generation.
- **Symbol Table**: Managing variable and function declarations and their types.

This project covers both **analysis** (breaking down Zara code into meaningful components) and **synthesis** (constructing executable code).

### Phases of Compilation

1. **Lexical Analysis (Current Phase)**:
   - The **Lexical Analyzer** (scanner) reads Zara code and breaks it into **tokens** such as keywords, operators, literals, identifiers, and data types.
   - Uses **regular expressions** to identify tokens and handle them as input for the next phase.
   
   [Learn more about the lexical analyzer in the "Lexical Analyzer" section below.]

2. **Syntax Analysis (Next Phase)**:
   - The **Parser** will check the sequence of tokens against Zara’s grammar to determine whether the syntax is valid. This will involve creating a **parse tree** or **abstract syntax tree (AST)**.
   
3. **Semantic Analysis**:
   - Ensures that Zara code is logically consistent. It will use the **symbol table** to check that variables are declared before use, type consistency in expressions, etc.

4. **Intermediate Code Generation**:
   - Transforms the Zara code into an intermediate representation, often closer to machine code but still independent of the final target architecture.

5. **Code Optimization**:
   - Applies techniques to make the intermediate code more efficient, reducing runtime or resource usage.

6. **Code Generation**:
   - The final stage where the optimized intermediate code is translated into target machine code or assembly language.

### Symbol Table
A **symbol table** will be implemented and used throughout the compiler’s phases. It will:
- Store variable and function declarations along with their types (integer, float, string, array, stack).
- Support operations like adding, updating, and retrieving symbols for use in later phases (semantic checks and code generation).

### Zara Programming Language Features
Zara supports the following features, which our compiler will handle:
- **Data Types**: `int`, `float`, `string`, `array`, `stack`
- **Control Structures**: `if-else`, `do-while`, `for` loops
- **Operators**: `=`, `*`, `/`, `-`, `>`, `==`
- **Sub-programs**: Methods and classes


### Requirements
- **C Compiler**: A GCC or Clang compiler is recommended.
- **Libraries**: `regex.h` for regular expressions (standard in most C environments).

### Phase 1: Lexical Analyzer

In the current phase, we’ve implemented a **Lexical Analyzer** for Zara. It breaks the input code into individual tokens, which are the smallest meaningful units of the program.

1. **Recognized Tokens**:
   - **Keywords**: `if`, `else`, `do`, `while`, `for`
   - **Operators**: `=`, `*`, `/`, `-`, `>`, `==`
   - **Data Types**: `int`, `float`, `string`, `array`, `stack`
   - **Identifiers**: Variable names and method names
   - **Literals**:
     - Integer literals: e.g., `10`
     - Float literals: e.g., `3.14`
     - String literals: e.g., `"hello"`

2. **Running the Lexical Analyzer**:
   - Compile and run the analyzer on sample Zara code embedded in the source file:
     ```bash
     gcc src/lexical_analyzer.c -o lexical_analyzer -lm
     ./lexical_analyzer
     ```
   
   - It will print out the recognized tokens along with their type:
     ```
     DataType: int
     Identifier: a
     Operator: =
     IntegerLiteral: 10
     Operator: ;
     ```

### Next Steps
- **Implement Syntax Analyzer**: Develop the parser to construct a parse tree and validate Zara's syntax.
- **Build the Symbol Table**: Manage variable and method declarations as part of the semantic analysis phase.
- **Complete all phases**: Move on to intermediate code generation, optimization, and final code generation.

### Contribution
This is a learning project in compiler construction. Contributions to extend its functionality and optimize the compiler are welcome. Please open issues or submit pull requests for improvements.


### Acknowledgments
This project is part of a compiler construction course, and it's being developed to better understand the complete compilation process for Zara.


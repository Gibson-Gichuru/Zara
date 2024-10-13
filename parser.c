#include "parser.h"

/**
 * @brief Initializes a parser with a given source string
 * 
 * @param source The source string to parse
 * @return A parser initialized with the given source string
 */
Parser InitParser(const char* source) {
    Parser parser;
    parser.lexer = InitLexer(source);
    parser.currentToken = GetNextToken(&parser.lexer);
    InitSymbolTable(&parser.symbolTable);
    return parser;
}

/**
 * @brief Advances the parser to the next token in the source string
 *
 * @param parser The parser to Advance
 */
void Advance(Parser* parser) {
    parser->currentToken = GetNextToken(&parser->lexer);
}

/**
 * @brief Checks if the current token matches the given type
 *
 * @param parser The parser instance
 * @param type The TokenType to Match
 *
 * @return 1 if the token matches, 0 if it does not
 */
int Match(Parser* parser, TokenType type) {
    return parser->currentToken.type == type;
}

/**
 * @brief Checks if the current token matches the given type and advances
 *        the parser to the next token if it does. If the token does not
 *        Match, prints an error message containing the given message and
 *        exits the program.
 *
 * @param parser The parser instance
 * @param type The TokenType to Match
 * @param errorMsg An error message to print if the token does not Match
 */
void Expect(Parser* parser, TokenType type, const char* errorMsg) {
    if (parser->currentToken.type == type) {
        Advance(parser);
    } else {
        fprintf(stderr, "Error: %s. Found '%s'\n", errorMsg, parser->currentToken.lexeme);
        exit(EXIT_FAILURE);
    }
}

// void ParseExpression(Parser* parser);
// void ParseStatement(Parser* parser);

/**
 * @brief Parses a Zara program
 *
 * The program consists of one or more functions. This function parses all
 * functions until the end of the source string is reached.
 *
 * @param parser The parser instance
 */
void ParseProgram(Parser* parser) {
    while (!Match(parser, TOKEN_EOF)) {
        ParseFunction(parser);
    }
}
/**
 * @brief Parses a Zara function
 *
 * The function consists of a return type, a name, a parameter list, and a
 * body. This function parses all of these components until the end of the
 * function is reached.
 *
 * @param parser The parser instance
 */
void ParseFunction(Parser* parser) {
    
    DataType funcType;
    if (Match(parser, TOKEN_INT)) {
        funcType = INTEGER;
    } else if (Match(parser, TOKEN_FLOAT)) {
        funcType = FLOAT;
    } else if (Match(parser, TOKEN_STRING)) {
        funcType = STRING;
    } else {
        fprintf(stderr, "Error: Expected function return type.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    if (!Match(parser, TOKEN_IDENTIFIER)) {
        fprintf(stderr, "Error: Expected function name.\n");
        exit(EXIT_FAILURE);
    }
    char funcName[50];
    strcpy(funcName, parser->currentToken.lexeme);
    Advance(parser);

   
    Expect(parser, TOKEN_SEPARATOR, "Expected '(' after function name");
    if (strcmp(parser->currentToken.lexeme, "(") != 0) {
        fprintf(stderr, "Error: Expected '('.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    if (!Match(parser, TOKEN_SEPARATOR) || strcmp(parser->currentToken.lexeme, ")") != 0) {
        ParseParameters(parser);
    }

    Expect(parser, TOKEN_SEPARATOR, "Expected ')' after parameters");
    if (strcmp(parser->currentToken.lexeme, ")") != 0) {
        fprintf(stderr, "Error: Expected ')'.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    Expect(parser, TOKEN_SEPARATOR, "Expected '{' to start function body");
    if (strcmp(parser->currentToken.lexeme, "{") != 0) {
        fprintf(stderr, "Error: Expected '{'.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    while (!Match(parser, TOKEN_SEPARATOR) || strcmp(parser->currentToken.lexeme, "}") != 0) {
        ParseStatement(parser);
    }

    Expect(parser, TOKEN_SEPARATOR, "Expected '}' to end function body");
    if (strcmp(parser->currentToken.lexeme, "}") != 0) {
        fprintf(stderr, "Error: Expected '}'.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    printf("Parsed function: %s\n", funcName);
}

/**
 * @brief Parses a parameter list in a function definition
 *
 * This function parses one or more parameters separated by commas.
 *
 * @param parser The parser instance
 */
void ParseParameters(Parser* parser) {
    ParseParameter(parser);
    while (Match(parser, TOKEN_SEPARATOR) && strcmp(parser->currentToken.lexeme, ",") == 0) {
        Advance(parser); // Skip ','
        ParseParameter(parser);
    }
}


/**
 * @brief Parses a single parameter in a function definition
 *
 * This function parses the parameter's type and name, and adds it to the symbol table.
 *
 * @param parser The parser instance
 */
void ParseParameter(Parser* parser) {
    DataType paramType;
    if (Match(parser, TOKEN_INT)) {
        paramType = INTEGER;
    } else if (Match(parser, TOKEN_FLOAT)) {
        paramType = FLOAT;
    } else if (Match(parser, TOKEN_STRING)) {
        paramType = STRING;
    } else {
        fprintf(stderr, "Error: Expected parameter type.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    if (!Match(parser, TOKEN_IDENTIFIER)) {
        fprintf(stderr, "Error: Expected parameter name.\n");
        exit(EXIT_FAILURE);
    }
    char paramName[50];
    strcpy(paramName, parser->currentToken.lexeme);
    Advance(parser);

    if (AddSymbol(&parser->symbolTable, paramName, paramType, NULL) != 0) {
        fprintf(stderr, "Error: Failed to add parameter '%s' to symbol table.\n", paramName);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Parses a statement in the current scope
 *
 * This function parses a single statement in the current scope, which may be a
 * declaration, an if statement, a for loop, a do-while loop, an assignment,
 * a function call, a return statement, or a block of statements.
 *
 * @param parser The parser instance
 */

void ParseStatement(Parser* parser) {
    if (Match(parser, TOKEN_INT) || Match(parser, TOKEN_FLOAT) || Match(parser, TOKEN_STRING)) {
        ParseDeclaration(parser);
    }
    else if (Match(parser, TOKEN_IF)) {
        ParseIfStatement(parser);
    }
    else if (Match(parser, TOKEN_FOR)) {
        ParseForLoop(parser);
    }
    else if (Match(parser, TOKEN_DO)) {
        ParseDoWhileLoop(parser);
    }
    else if (Match(parser, TOKEN_IDENTIFIER)) {
        Token nextToken = GetNextToken(&parser->lexer);
        parser->lexer.position -= strlen(nextToken.lexeme);
        if (strcmp(nextToken.lexeme, "(") == 0) {
            ParseFunctionCall(parser);
        }
        else {
            ParseAssignment(parser);
        }
    }
    else if (Match(parser, TOKEN_RETURN)) {
        ParseReturnStatement(parser);
    }
    else if (Match(parser, TOKEN_SEPARATOR) && strcmp(parser->currentToken.lexeme, "{") == 0) {
        Advance(parser);
        while (!Match(parser, TOKEN_SEPARATOR) || strcmp(parser->currentToken.lexeme, "}") != 0) {
            ParseStatement(parser);
        }
        Expect(parser, TOKEN_SEPARATOR, "Expected '}' to close block");
        Advance(parser);
    }
    else {
        fprintf(stderr, "Error: Unexpected token '%s' in statement.\n", parser->currentToken.lexeme);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Parses a variable declaration in a statement
 *
 * This function parses a declaration statement, which consists of a type, a variable name, and
 * an optional initializer expression. The type must be one of the basic types (int, float, string),
 * and the variable name must be an identifier. The initializer expression is optional, and if it is
 * not present, the variable is initialized with a default value of 0.
 *
 * @param parser The parser instance
 */
void ParseDeclaration(Parser* parser) {
    DataType declType;
    if (Match(parser, TOKEN_INT)) {
        declType = INTEGER;
    } else if (Match(parser, TOKEN_FLOAT)) {
        declType = FLOAT;
    } else if (Match(parser, TOKEN_STRING)) {
        declType = STRING;
    } else {
        fprintf(stderr, "Error: Unknown declaration type.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    if (!Match(parser, TOKEN_IDENTIFIER)) {
        fprintf(stderr, "Error: Expected variable name in declaration.\n");
        exit(EXIT_FAILURE);
    }
    char varName[50];
    strcpy(varName, parser->currentToken.lexeme);
    Advance(parser);

    void* value = NULL;
    if (Match(parser, TOKEN_OPERATOR) && strcmp(parser->currentToken.lexeme, "=") == 0) {
        Advance(parser);
        ParseExpression(parser);
    }

    Expect(parser, TOKEN_SEPARATOR, "Expected ';' after declaration");
    if (strcmp(parser->currentToken.lexeme, ";") != 0) {
        fprintf(stderr, "Error: Expected ';' after declaration.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    if (AddSymbol(&parser->symbolTable, varName, declType, NULL) != 0) {
        fprintf(stderr, "Error: Failed to add variable '%s' to symbol table.\n", varName);
        exit(EXIT_FAILURE);
    }

    printf("Declared variable: %s\n", varName);
}


/**
 * @brief Parses an assignment statement in a statement
 *
 * This function parses an assignment statement, which consists of an identifier, an '=' operator, an
 * expression, and a ';'. The identifier must be an existing variable in the symbol table, and the
 * expression is expected to be a valid expression. The expression is evaluated and the value is assigned
 * to the variable.
 *
 * @param parser The parser instance
 */
void ParseAssignment(Parser* parser) {
    char varName[50];
    strcpy(varName, parser->currentToken.lexeme);
    Advance(parser);

    Expect(parser, TOKEN_OPERATOR, "Expected '=' in assignment");
    if (strcmp(parser->currentToken.lexeme, "=") != 0) {
        fprintf(stderr, "Error: Expected '=' in assignment.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    ParseExpression(parser);

    Expect(parser, TOKEN_SEPARATOR, "Expected ';' after assignment");
    if (strcmp(parser->currentToken.lexeme, ";") != 0) {
        fprintf(stderr, "Error: Expected ';' after assignment.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    printf("Assigned to variable: %s\n", varName);
}


/**
 * @brief Parses an if statement in a statement
 *
 * This function parses an if statement, which consists of an 'if' keyword, a condition expression
 * enclosed in parentheses, and a 'then' statement. The condition expression is expected to be a
 * valid expression. The 'then' statement is expected to be a valid statement. An optional 'else'
 * statement can also be present.
 *
 * @param parser The parser instance
 */
void ParseIfStatement(Parser* parser) {
    Expect(parser, TOKEN_IF, "Expected 'if'");
    if (strcmp(parser->currentToken.lexeme, "if") != 0) {
        fprintf(stderr, "Error: Expected 'if'.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);
    Expect(parser, TOKEN_SEPARATOR, "Expected '(' after 'if'");
    if (strcmp(parser->currentToken.lexeme, "(") != 0) {
        fprintf(stderr, "Error: Expected '('.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    ParseExpression(parser);
    Expect(parser, TOKEN_SEPARATOR, "Expected ')' after condition");
    if (strcmp(parser->currentToken.lexeme, ")") != 0) {
        fprintf(stderr, "Error: Expected ')'.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    ParseStatement(parser);

    if (Match(parser, TOKEN_ELSE)) {
        Advance(parser); 
        ParseStatement(parser);
    }

    printf("Parsed if statement.\n");
}


/**
 * @brief Parses a for loop in a statement
 *
 * This function parses a for loop, which consists of a 'for' keyword, an initialization
 * (declaration or assignment), a condition expression, an increment expression, and a loop body.
 * The initialization, condition, and increment expressions are expected to be valid
 * expressions. The loop body is expected to be a valid statement.
 *
 * @param parser The parser instance
 */
void ParseForLoop(Parser* parser) {
    Expect(parser, TOKEN_FOR, "Expected 'for'");
    if (strcmp(parser->currentToken.lexeme, "for") != 0) {
        fprintf(stderr, "Error: Expected 'for'.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    Expect(parser, TOKEN_SEPARATOR, "Expected '(' after 'for'");
    if (strcmp(parser->currentToken.lexeme, "(") != 0) {
        fprintf(stderr, "Error: Expected '('.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    if (Match(parser, TOKEN_INT) || Match(parser, TOKEN_FLOAT) || Match(parser, TOKEN_STRING) || Match(parser, TOKEN_IDENTIFIER)) {
        ParseStatement(parser);
    }

    if (!Match(parser, TOKEN_SEPARATOR) || strcmp(parser->currentToken.lexeme, ";") != 0) {
        ParseExpression(parser);
    }
    Expect(parser, TOKEN_SEPARATOR, "Expected ';' after for-loop condition");
    if (strcmp(parser->currentToken.lexeme, ";") != 0) {
        fprintf(stderr, "Error: Expected ';' after for-loop condition.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    if (!Match(parser, TOKEN_SEPARATOR) || strcmp(parser->currentToken.lexeme, ")") != 0) {
        ParseAssignment(parser);
    }

    Expect(parser, TOKEN_SEPARATOR, "Expected ')' after for-loop increment");
    if (strcmp(parser->currentToken.lexeme, ")") != 0) {
        fprintf(stderr, "Error: Expected ')'.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    ParseStatement(parser);

    printf("Parsed for loop.\n");
}

/**
 * @brief Parses a do-while loop in a statement
 *
 * This function parses a do-while loop, which consists of a 'do' keyword, a loop body, a 'while'
 * keyword, a condition expression, and a ';'. The loop body is expected to be a valid statement.
 * The condition expression is expected to be a valid expression.
 *
 * @param parser The parser instance
 */
void ParseDoWhileLoop(Parser* parser) {
    Expect(parser, TOKEN_DO, "Expected 'do'");
    if (strcmp(parser->currentToken.lexeme, "do") != 0) {
        fprintf(stderr, "Error: Expected 'do'.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    ParseStatement(parser);

    Expect(parser, TOKEN_WHILE, "Expected 'while' after 'do' loop body");
    if (strcmp(parser->currentToken.lexeme, "while") != 0) {
        fprintf(stderr, "Error: Expected 'while'.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    Expect(parser, TOKEN_SEPARATOR, "Expected '(' after 'while'");
    if (strcmp(parser->currentToken.lexeme, "(") != 0) {
        fprintf(stderr, "Error: Expected '('.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    ParseExpression(parser);

    Expect(parser, TOKEN_SEPARATOR, "Expected ')' after condition");
    if (strcmp(parser->currentToken.lexeme, ")") != 0) {
        fprintf(stderr, "Error: Expected ')'.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    Expect(parser, TOKEN_SEPARATOR, "Expected ';' after do-while loop");
    if (strcmp(parser->currentToken.lexeme, ";") != 0) {
        fprintf(stderr, "Error: Expected ';' after do-while loop.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    printf("Parsed do-while loop.\n");
}

/**
 * @brief Parses a function call in a statement
 *
 * This function parses a function call, which consists of a function name, an argument list enclosed
 * in parentheses, and a ';'. The argument list is optional and can contain any number of valid
 * expressions separated by commas. The function name must be an existing function in the symbol
 * table, and the argument list must Match the expected parameter list of the function.
 *
 * @param parser The parser instance
 */
void ParseFunctionCall(Parser* parser) {
    char funcName[50];
    strcpy(funcName, parser->currentToken.lexeme);
    Advance(parser);

    Expect(parser, TOKEN_SEPARATOR, "Expected '(' in function call");
    if (strcmp(parser->currentToken.lexeme, "(") != 0) {
        fprintf(stderr, "Error: Expected '('.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    if (!Match(parser, TOKEN_SEPARATOR) || strcmp(parser->currentToken.lexeme, ")") != 0) {
        ParseExpression(parser);
        while (Match(parser, TOKEN_SEPARATOR) && strcmp(parser->currentToken.lexeme, ",") == 0) {
            Advance(parser); // Skip ','
            ParseExpression(parser);
        }
    }

    Expect(parser, TOKEN_SEPARATOR, "Expected ')' in function call");
    if (strcmp(parser->currentToken.lexeme, ")") != 0) {
        fprintf(stderr, "Error: Expected ')'.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    Expect(parser, TOKEN_SEPARATOR, "Expected ';' after function call");
    if (strcmp(parser->currentToken.lexeme, ";") != 0) {
        fprintf(stderr, "Error: Expected ';' after function call.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    printf("Parsed function call: %s\n", funcName);
}

/**
 * @brief Parses a return statement in a statement
 *
 * This function parses a return statement, which consists of a 'return' keyword, an optional
 * expression, and a ';'. The expression is evaluated and its value is returned from the
 * function.
 *
 * @param parser The parser instance
 */
void ParseReturnStatement(Parser* parser) {
    Expect(parser, TOKEN_RETURN, "Expected 'return'");
    if (strcmp(parser->currentToken.lexeme, "return") != 0) {
        fprintf(stderr, "Error: Expected 'return'.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    if (!Match(parser, TOKEN_SEPARATOR) || strcmp(parser->currentToken.lexeme, ";") != 0) {
        ParseExpression(parser);
    }

    Expect(parser, TOKEN_SEPARATOR, "Expected ';' after return statement");
    if (strcmp(parser->currentToken.lexeme, ";") != 0) {
        fprintf(stderr, "Error: Expected ';' after return statement.\n");
        exit(EXIT_FAILURE);
    }
    Advance(parser);

    printf("Parsed return statement.\n");
}

/**
 * @brief Parses an expression in a statement
 *
 * This function parses an expression, which consists of one or more terms separated by
 * '+' or '-' operators. Each term is parsed by calling ParseTerm.
 *
 * @param parser The parser instance
 */
void ParseExpression(Parser* parser) {
    ParseTerm(parser);
    while (Match(parser, TOKEN_OPERATOR) && (strcmp(parser->currentToken.lexeme, "+") == 0 || strcmp(parser->currentToken.lexeme, "-") == 0)) {
        Advance(parser); // Skip '+' or '-'
        ParseTerm(parser);
    }
}

/**
 * @brief Parses a term in an expression
 *
 * This function parses a term, which consists of one or more factors separated by
 * '*' or '/' or '%' operators. Each factor is parsed by calling ParseFactor.
 *
 * @param parser The parser instance
 */
void ParseTerm(Parser* parser) {
    ParseFactor(parser);
    while (Match(parser, TOKEN_OPERATOR) && (strcmp(parser->currentToken.lexeme, "*") == 0 || strcmp(parser->currentToken.lexeme, "/") == 0 || strcmp(parser->currentToken.lexeme, "%") == 0)) {
        Advance(parser); // Skip '*', '/', '%'
        ParseFactor(parser);
    }
}

/**
 * @brief Parses a factor in an expression
 *
 * This function parses a factor, which consists of an expression enclosed in
 * parentheses, a variable name, a number, or a string literal. If the factor
 * is an expression enclosed in parentheses, ParseExpression is called to parse
 * the expression. If the factor is a variable name, it is parsed by calling
 * ParseIdentifier. If the factor is a number or a string literal, it is
 * parsed by calling ParseNumber or ParseStringLiteral, respectively.
 *
 * @param parser The parser instance
 */
void ParseFactor(Parser* parser) {
    if (Match(parser, TOKEN_SEPARATOR) && strcmp(parser->currentToken.lexeme, "(") == 0) {
        Advance(parser); 
        ParseExpression(parser);
        Expect(parser, TOKEN_SEPARATOR, "Expected ')' after expression");
        if (strcmp(parser->currentToken.lexeme, ")") != 0) {
            fprintf(stderr, "Error: Expected ')'.\n");
            exit(EXIT_FAILURE);
        }
        Advance(parser);
    }
    else if (Match(parser, TOKEN_IDENTIFIER)) {
        Advance(parser);
    }
    else if (Match(parser, TOKEN_NUMBER) || Match(parser, TOKEN_FLOAT) || Match(parser, TOKEN_STRING_LITERAL)) {
        Advance(parser);
    }
    else {
        fprintf(stderr, "Error: Unexpected token '%s' in expression.\n", parser->currentToken.lexeme);
        exit(EXIT_FAILURE);
    }
}
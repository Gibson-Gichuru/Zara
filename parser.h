#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "symbol.h"

typedef struct {
    Lexer lexer;
    Token currentToken;
    SymbolTable symbolTable;
} Parser;


Parser InitParser(const char* source);
void Advance(Parser* parser);
int Match(Parser* parser, TokenType type);
void Expect(Parser* parser, TokenType type, const char* errorMsg);
void ParseProgram(Parser* parser);
void ParseFunction(Parser* parser);
void ParseParameters(Parser* parser);
void ParseParameter(Parser* parser);
void ParseStatement(Parser* parser);
void ParseDeclaration(Parser* parser);
void ParseAssignment(Parser* parser);
void ParseIfStatement(Parser* parser);
void ParseForLoop(Parser* parser);
void ParseDoWhileLoop(Parser* parser);
void ParseFunctionCall(Parser* parser);
void ParseReturnStatement(Parser* parser);
void ParseExpression(Parser* parser);
void ParseTerm(Parser* parser);
void ParseFactor(Parser* parser);
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"

/**
 * @brief Initializes a lexer with the given source string
 * 
 * @param source The source string to lex
 * @return A lexer initialized with the given source string
 */


Token tokens[MAX_TOKENS];

int tokenCount = 0;

Lexer InitLexer(const char* source) {
    Lexer lexer;
    lexer.source = source;
    lexer.position = 0;

    return lexer;
}

void AddToken(TokenType type, const char* lexeme) {
    if (tokenCount >= MAX_TOKENS) {
        printf("Too many tokens!\n");
        return;
    }
    tokens[tokenCount].type = type;
    strncpy(tokens[tokenCount].lexeme, lexeme, MAX_TOKENS);
    tokenCount++;
}

/**
 * @brief Checks if a given string is a keyword in the Zara language and returns
 *        the corresponding token type if it is.
 *
 * @param str The string to check.
 * @param type A pointer to a TokenType that will be set to the type of the
 *             keyword if str is a keyword. If str is not a keyword, the value
 *             of type is undefined.
 *
 * @return 1 if str is a keyword, 0 if it is not.
 */
int IsKeyword(const char* str, TokenType* type) {

    if(strcmp(str, "int") == 0) {
        *type = TOKEN_INT;
        return 1;
    }
    if(strcmp(str, "float") == 0) {
        *type = TOKEN_INT;
        return 1;
    }
    if(strcmp(str, "string") == 0) {
        *type = TOKEN_STRING;
        return 1;
    }
    if(strcmp(str, "if") == 0) {
        *type = TOKEN_IF;
        return 1;
    }
    if(strcmp(str, "else") == 0) {
        *type = TOKEN_ELSE;
        return 1;
    }
    if(strcmp(str, "for") == 0) {
        *type = TOKEN_FOR;
        return 1;
    }
    if(strcmp(str, "do") == 0) {
        *type = TOKEN_DO;
        return 1;
    }
    if(strcmp(str, "while") == 0) {
        *type = TOKEN_WHILE;
        return 1;
    }
    if(strcmp(str, "return") == 0) {
        *type = TOKEN_RETURN;
        return 1;
    }

    return 0;
    
}

/**
 * @brief Get the next token from the lexer's source string
 * 
 * This function implements a lexical analyzer. It returns a token
 * from the source string, skipping any whitespace characters. The
 * type of the token is automatically determined by its content.
 * 
 * @param lexer The lexer to get the token from
 * @return The next token from the lexer's source string
 */
Token GetNextToken(Lexer* lexer) {

    Token token;
    token.type = TOKEN_UNKNOWN;
    token.lexeme[0] = '\0';

    while(lexer->source[lexer->position] != '\0') {
        char current = lexer->source[lexer->position];

        if(isspace(current)) {
            lexer->position++;
            continue;
        }

        if(isalpha(current) || current == '_') {
            int start = lexer->position;

            while (isalnum(lexer->source[lexer->position]) || lexer->source[lexer->position] == '_') {
                lexer->position++;
            }

            int length = lexer->position - start;
            strncpy(token.lexeme, lexer->source + start, length);
            token.lexeme[length] = '\0';
            
            if(IsKeyword(token.lexeme, &token.type)) {
                return token;
            }
            else {
                token.type = TOKEN_IDENTIFIER;
                return token;
            }
        }

        if(isdigit(current)) {
            int start = lexer->position;
            int hasDot = 0;
            while (isdigit(lexer->source[lexer->position]) || lexer->source[lexer->position] == '.') {
                if (lexer->source[lexer->position] == '.') {
                    if (hasDot) break; // Second dot encountered
                    hasDot = 1;
                }
                lexer->position++;
            }
            int length = lexer->position - start;
            strncpy(token.lexeme, lexer->source + start, length);
            token.lexeme[length] = '\0';
            token.type = hasDot ? TOKEN_FLOAT : TOKEN_NUMBER;
            return token;
        }

        if (current == '\"') {
            lexer->position++; // Skip opening quote
            int start = lexer->position;
            while (lexer->source[lexer->position] != '\"' && lexer->source[lexer->position] != '\0') {
                lexer->position++;
            }
            int length = lexer->position - start;
            strncpy(token.lexeme, lexer->source + start, length);
            token.lexeme[length] = '\0';
            token.type = TOKEN_STRING_LITERAL;
            if (lexer->source[lexer->position] == '\"') {
                lexer->position++; // Skip closing quote
            }
            return token;
        }

        if (strchr("=+-*/%<>!", current)) {
            int start = lexer->position;
            lexer->position++;
            // Handle two-character operators
            if ((current == '=' || current == '!' || current == '<' || current == '>') && lexer->source[lexer->position] == '=') {
                lexer->position++;
                int length = lexer->position - start;
                strncpy(token.lexeme, lexer->source + start, length);
                token.lexeme[length] = '\0';
                token.type = TOKEN_OPERATOR;
                return token;
            } else {
                token.lexeme[0] = current;
                token.lexeme[1] = '\0';
                token.type = TOKEN_OPERATOR;
                return token;
            }
        }

        if (strchr("();{},", current)) {
            token.lexeme[0] = current;
            token.lexeme[1] = '\0';
            token.type = TOKEN_SEPARATOR;
            lexer->position++;
            return token;
        }

        lexer->position++;
        token.type = TOKEN_UNKNOWN;
        return token;
    }

    token.type = TOKEN_EOF;
    strcpy(token.lexeme, "EOF");
    return token;
}

void Tokenize(char* line) {
    char* token = strtok(line, " \t");

    while (token != NULL) {
        TokenType type;

        // Check if the token is a keyword
        if (IsKeyword(token, &type)) {
            AddToken(type, token);
        }
        // Check if the token is an operator (this can be extended)
        else if (strcmp(token, "=") == 0 || strcmp(token, "+") == 0 || strcmp(token, "-") == 0 || 
                 strcmp(token, "*") == 0 || strcmp(token, "/") == 0 || strcmp(token, ">") == 0) {
            AddToken(TOKEN_OPERATOR, token);
        }
        // Check if the token is a number (literal)
        else if (isdigit(token[0])) {
            AddToken(TOKEN_NUMBER, token);
        }
        // Otherwise, assume it's an identifier (variable or function name)
        else {
            AddToken(TOKEN_IDENTIFIER, token);
        }

        token = strtok(NULL, " \t");
    }
}
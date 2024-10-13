#define MAX_NAME_LENGTH 100
#define MAX_TOKENS 100

typedef enum {
    TOKEN_EOF,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_DO,
    TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING_LITERAL,
    TOKEN_OPERATOR,
    TOKEN_SEPARATOR,
    TOKEN_UNKNOWN
} TokenType;

typedef struct 
{
    TokenType type;
    char lexeme[MAX_NAME_LENGTH];

} Token;

typedef struct
{
    const char* source;
    int position;
} Lexer;

Lexer InitLexer(const char* source);
Token GetNextToken(Lexer* lexer);
int IsKeyword(const char* str, TokenType* type);
void Tokenize(char* line);
void AddToken(TokenType type, const char* lexeme);
/**
 * lexer.h - prototypes for the C++ lexer
 *
 * Implementation notes
 * 
 * Initial version: Dec 30th 2020
 */

#include "my_types.h"

/* 
 * Type for function that reads characters.  Use so we can use any source for reading characters 
 */

typedef MyInt (*CharReader)(void);


/*
 * Sets the function that reads the characters by the lexer
 */

void setCharReader(CharReader funcCharReader);

/*
 * Tokens returned by the lexer
 */

typedef enum {
    LT_ID = 0,
    LT_INTEGER,
    /* to complete... */
    LT_EOF
} LexerTokenKind;

typedef struct {
    LexerTokenKind kind;
    union {
        /* kind == LT_ID */
        MyString id;
    } tokenInfo;
} LexerToken;

typedef LexerToken* LexerTokenPtr;

/*
 * Consume the next token
 */
void consumeToken(void);
void consumeTokens(MyInt lookahead);

/*
 * Peek the next token or a token with a lookahead
 */
LexerTokenPtr peekNextToken(void);
LexerTokenPtr peekToken(MyInt lookahead);



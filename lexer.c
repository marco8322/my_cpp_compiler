/**
 * lexer.c - lexer for the C++ tokens
 */

#include "lexer.h"
#include <stdio.h>

typedef enum {
    NUM_IS_OCTAL,
    NUM_IS_HEX,
    NUM_IS_DEC
} NumKind;

static LexerTokenPtr readZeroStartedToken();
static LexerTokenPtr readNumNonZeroStartedToken();
static LexerTokenPtr readRestNumberToken(MyInt numKind);
static MyBool isSpaceChar(MyInt theChar);

/*
 * Current reader
 */

static CharReader currentCharReader;

/*
 * Set the char reader
 */
void setCharReader(CharReader funcCharReader)
{
    currentCharReader = funcCharReader;
}


/*
 * Main routine to read the next token
 */

static LexerTokenPtr getNextToken()
{
    MyInt nextChar = currentCharReader();

    if( nextChar == '0' ) {
        return readZeroStartedToken();
    }
    else if( nextChar >= '1' && nextChar <= '9' ) {
        return readNumNonZeroStartedToken();
    }
    else if( nextChar == EOF ) {
        return createToken(LT_EOF);
    }
}



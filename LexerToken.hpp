// LexerToken.hpp
//
// Author: Marco Jacques
//
// Lexer tokens definition
//

#pragma once

#include "SourcePosition.hpp"
#include <string>


/**
 * Interface for all tokens.  
 */
class LexerToken {
public:
    enum Kind {
        IDENTIFIER = 1,
        STRING_LITERAL,
        INTEGER_LITERAL,
        FLOAT_LITERAL,

        /* keywords */
        VOID,
        SIGNED, UNSIGNED,
        CHAR, SHORT, INT, LONG,
        FLOAT, DOUBLE,
        STRUCT, UNION, ENUM,
        CONST, VOLATILE,
        SIZEOF,

        IF, ELSE,
        WHILE, DO, FOR,
        GOTO, BREAK, CONTINUE,
        SWITCH, CASE, DEFAULT,
        AUTO, REGISTER,
        TYPEDEF, RETURN,
        EXTERN, STATIC,

        /* other tokens */
        BEGIN_TOKEN_OP,
        LEFT_PARAR, RIGHT_PARAR, LEFT_BRACKET, RIGHT_BRACKET,
        DOT, LEFT_ARROW,
        INCR, DECR,
        COMMA,
        BIT_AND, BIT_IOR, BIT_XOR, BIT_NOT,
        BOOL_AND, BOOL_OR, BOOL_NOT,
        ADD, SUB, MUL, DIV, MOD,
        SHIFT_LEFT,
        SHIFT_RIGHT,
        LT, GT, LE, GE, EQUAL, NOT_EQUAL,
        QUESTION_MARK, COLON, DOT_DOT_DOT,
        
        ASSIGN,
        MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN, ADD_ASSIGN, SUB_ASSIGN,
        SHIFT_LEFT_ASSIGN, SHIFT_RIGHT_ASSIGN,
        BIT_AND_ASSIGN, BIT_XOR_ASSIGN, BIT_IOR_ASSIGN,
        END_TOKEN_OP,

        UNKNOWN,

        END_OF_FILE
    };

    Kind getKind() { return kind; }
    LexerToken(Kind kind_) 
        : kind(kind_)
    { 
    }

    virtual ~LexerToken() = 0;

private:
    Kind kind;
};

using LexerTokenPtr = std::shared_ptr<LexerToken>;


/**
 * Class for simple tokens not required having extra information
 */
class SimpleToken : public LexerToken {
public:
    SimpleToken(LexerToken::Kind kind) : LexerToken(kind) { }
    ~SimpleToken() = default;
};

/**
 * Class for tokens representing identifiers
 */
class IdToken : public LexerToken {
public:
    IdToken(const std::string&) : LexerToken(LexerToken::IDENTIFIER) { }
    virtual ~IdToken() = default;
};

/**
 * Class for tokens representing integer values
 */
class IntLiteralToken : public LexerToken {
public:
    IntLiteralToken() : LexerToken(LexerToken::INTEGER_LITERAL) { }
    ~IntLiteralToken() = default;
};

/**
 * Class for tokens representing string literals
 */
class StringLiteralToken : public LexerToken {
public:
    StringLiteralToken() : LexerToken(LexerToken::STRING_LITERAL) { }
    ~StringLiteralToken() = default;
};

/**
 * Class for unknown tokens
 */
class UnknownToken : public LexerToken {
    int theChar;
    
public:
    UnknownToken(int theChar_) : LexerToken(LexerToken::UNKNOWN), theChar(theChar_) { }
    ~UnknownToken() = default;

    int getUnknownChar() { return theChar; }
};


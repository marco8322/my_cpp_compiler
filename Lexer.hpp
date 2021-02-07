
#pragma once

#include <memory>


class LexerToken {
public:
    enum Kind {
        IDENTIFIER,
        STRING_LITERAL,
        INTEGER_LITERAL,
        FLOAT_LITERAL,

        /* keywords */
        VOID,
        CHAR,
        SIGNED,
        UNSIGNED,
        SHORT,
        INT,
        LONG,
        STRUCT,
        UNION,
        CONST,
        VOLATILE,
        SIZEOF,

        /* other tokens */
        LEFT_PARAR,
        RIGHT_PARAR,
        LEFT_BRACKET,
        RIGHT_BRACKET,
        DOT,
        LEFT_ARROW,
        INCR,
        DECR,
        COMMA,
        BIT_AND,
        BIT_IOR,
        BIT_XOR,
        BIT_NOT,
        BOOL_AND,
        BOOL_OR,
        BOOL_NOT,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        SHIFT_LEFT,
        SHIFT_RIGHT,
        LT,
        GT,
        LE,
        GE,
        EQUAL,
        NOT_EQUAL,
        QUESTION_MARK,
        COLON,
        
        ASSIGN,
        MUL_ASSIGN,
        DIV_ASSIGN,
        MOD_ASSIGN,
        ADD_ASSIGN,
        SUB_ASSIGN,
        SHIFT_LEFT_ASSIGN,
        SHIFT_RIGHT_ASSIGN,
        BIT_AND_ASSIGN,
        BIT_XOR_ASSIGN,
        BIT_IOR_ASSIGN
    };

    Kind getKind() { return kind; }
    LexerToken(Kind kind_ ) : kind(kind_) { }
    virtual ~LexerToken();

private:
    Kind kind;

};

class Lexer {
public:
    virtual std::shared_ptr<LexerToken> nextToken() = 0;
    virtual std::shared_ptr<LexerToken> peekToken() = 0;
    virtual std::shared_ptr<LexerToken> acceptToken(LexerToken::Kind) = 0;
};

using LexerTokenPtr = std::shared_ptr<LexerToken>;

class IdToken : public LexerToken {

};


class IntLiteralToken : public LexerToken {

};

class StringLiteralToken : public LexerToken {

};


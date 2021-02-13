
#pragma once

#include <memory>
#include <map>
#include <string>
#include "Message.hpp"
#include "LexerToken.hpp"
#include "CharReader.hpp"

/**
 * Interface for lexers
 */
class Lexer {
public:
    virtual LexerTokenPtr nextToken() = 0;
    virtual LexerTokenPtr peekToken() = 0;
    virtual LexerTokenPtr acceptToken(LexerToken::Kind) = 0;
};



using KeywordsMap = std::map<const std::string, LexerTokenPtr>;

/**
 * Implementation for the C90 lexer
 */
class C90Lexer : public Lexer {
public:
    virtual LexerTokenPtr nextToken() override;
    virtual LexerTokenPtr peekToken() override;
    virtual LexerTokenPtr acceptToken(LexerToken::Kind) override;

    C90Lexer(const std::shared_ptr<CharReader>& charReader_, const std::shared_ptr<Message>& msg_);

protected:
 
    LexerTokenPtr readIdOrKeyword();
    LexerTokenPtr readNumber();
    LexerTokenPtr readOtherToken();
    void addC90KeywordsAndOperators();
    void skipWhiteSpaces();

    std::shared_ptr<CharReader> charReader;
    LexerTokenPtr theNextToken;
    std::shared_ptr<Message> msg;
    KeywordsMap keywords;
    KeywordsMap operators;
};


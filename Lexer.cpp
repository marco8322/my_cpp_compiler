#include "Lexer.hpp"
#include <cctype>
#include <string>

/**
 * Constructor: initialize the lexer for C90
 */
C90Lexer::C90Lexer(
        const std::shared_ptr<CharReader>& charReader_, 
        const std::shared_ptr<Message>& msg_
        ) 
    : charReader(charReader_), theNextToken(nullptr), msg(msg_)
{ 
    addC90Keywords();
}

/**
 * Return the next token
 */
std::shared_ptr<LexerToken> C90Lexer::nextToken()
{
    std::shared_ptr<LexerToken> result = peekToken();
    theNextToken = nullptr;

    return result;
}

/**
 * Lookahead to the next token
 */
std::shared_ptr<LexerToken> C90Lexer::peekToken()
{
    if( theNextToken == nullptr ) {

        skipWhiteSpaces();
        int nextChar = charReader->peekNextChar();

        // If this is a letter or _, this is either a id or a keyword
        //
        if( std::isalpha(nextChar) || nextChar == '_' ) {
            theNextToken = readIdOrKeyword();
        }
    
        // else if this is a number, get a number
        //
        else if( std::isdigit(nextChar) ) {
            theNextToken = readNumber();
        }

        // EOF
        //
        else if( nextChar == EOF ) {
            theNextToken = std::make_shared<SimpleToken>(LexerToken::END_OF_FILE);
        }

        // Check for another token
        //
        else {
            theNextToken = readOtherToken();
        }
    }

    return theNextToken;
}

/**
 * Accept the token.  If the kind is not the one expected, issue an error
 */
std::shared_ptr<LexerToken> C90Lexer::acceptToken(LexerToken::Kind expectedKind) 
{
    std::shared_ptr<LexerToken> result = peekToken();
    if( result->getKind() != expectedKind ) {
        msg->error(Message::ERROR_EXPECTED_TOKEN, {"<need appropriate string>"});
        return nullptr;
    }

    theNextToken = nullptr;
    return result;
}

/**
 * Read an identifier or keyword
 */
std::shared_ptr<LexerToken> C90Lexer::readIdOrKeyword()
{
    std::string idString;
 
    int nextChar = charReader->peekNextChar();

    while( std::isalnum(nextChar) || nextChar == '_' ) {
        idString.push_back(nextChar);
        charReader->getNextChar();
        nextChar = charReader->peekNextChar();
    }

    using keywordIterator = KeywordsMap::const_iterator;
    keywordIterator iterKeyword = keywords.find(idString);
    if( iterKeyword != keywords.end() ) {
        return iterKeyword->second;
    }
    else {
        return std::make_shared<IdToken>(idString);
    }
}

/**
 * Read a number (TO IMPLEMEMT)
 */
std::shared_ptr<LexerToken> C90Lexer::readNumber()
{
    return nullptr;
}

/**
 * Read other token
 */
std::shared_ptr<LexerToken> C90Lexer::readOtherToken()
{
    int firstChar = charReader->getNextChar();
    int nextChar1 = charReader->peekNextChar();

    switch( firstChar ) {
        case '+': {
            if( nextChar1 == '+' ) {
                charReader->getNextChar();
                return std::make_shared<SimpleToken>(LexerToken::INCR);
            } else if( nextChar1 == '=' ) {
                charReader->getNextChar();
                return std::make_shared<SimpleToken>(LexerToken::ADD_ASSIGN);
            } else {
                return std::make_shared<SimpleToken>(LexerToken::ADD);
            }
        }

        case '=': {
            if( nextChar1 == '=' ) {
                charReader->getNextChar();
                return std::make_shared<SimpleToken>(LexerToken::EQUAL);
            } else {
                return std::make_shared<SimpleToken>(LexerToken::ASSIGN);
            }
        }
    }

    return nullptr;
}

/**
 * Skip whitespaces
 */
void C90Lexer::skipWhiteSpaces()
{
    int nextChar = charReader->peekNextChar();

    while( std::isspace(nextChar) ) {
        charReader->getNextChar();
        nextChar = charReader->peekNextChar();
    }
}

/**
 * Initialize the keywords map with the C90 keywords
 */
void C90Lexer::addC90Keywords()
{
    keywords["sizeof"]   = std::make_shared<SimpleToken>(LexerToken::SIZEOF);
    keywords["char"]     = std::make_shared<SimpleToken>(LexerToken::CHAR);
    keywords["short"]    = std::make_shared<SimpleToken>(LexerToken::SHORT);
    keywords["int"]      = std::make_shared<SimpleToken>(LexerToken::INT);
    keywords["long"]     = std::make_shared<SimpleToken>(LexerToken::LONG);
    keywords["signed"]   = std::make_shared<SimpleToken>(LexerToken::SIGNED);
    keywords["unsigned"] = std::make_shared<SimpleToken>(LexerToken::UNSIGNED);
    keywords["void"]     = std::make_shared<SimpleToken>(LexerToken::VOID);
    keywords["float"]    = std::make_shared<SimpleToken>(LexerToken::FLOAT);
    keywords["double"]   = std::make_shared<SimpleToken>(LexerToken::DOUBLE);
    keywords["struct"]   = std::make_shared<SimpleToken>(LexerToken::STRUCT);
    keywords["union"]    = std::make_shared<SimpleToken>(LexerToken::UNION);
    keywords["enum"]     = std::make_shared<SimpleToken>(LexerToken::ENUM);
    keywords["const"]    = std::make_shared<SimpleToken>(LexerToken::CONST);
    keywords["volatile"] = std::make_shared<SimpleToken>(LexerToken::VOLATILE);
    keywords["extern"]   = std::make_shared<SimpleToken>(LexerToken::EXTERN);
    keywords["static"]   = std::make_shared<SimpleToken>(LexerToken::STATIC);

    keywords["if"]       = std::make_shared<SimpleToken>(LexerToken::IF);
    keywords["else"]     = std::make_shared<SimpleToken>(LexerToken::ELSE);
    keywords["do"]       = std::make_shared<SimpleToken>(LexerToken::DO);
    keywords["for"]      = std::make_shared<SimpleToken>(LexerToken::FOR);
    keywords["while"]    = std::make_shared<SimpleToken>(LexerToken::WHILE);
    keywords["goto"]     = std::make_shared<SimpleToken>(LexerToken::GOTO);
    keywords["break"]    = std::make_shared<SimpleToken>(LexerToken::BREAK);
    keywords["continue"] = std::make_shared<SimpleToken>(LexerToken::CONTINUE);
    keywords["switch"]   = std::make_shared<SimpleToken>(LexerToken::SWITCH);
    keywords["case"]     = std::make_shared<SimpleToken>(LexerToken::CASE);
    keywords["default"]  = std::make_shared<SimpleToken>(LexerToken::DEFAULT);
    keywords["auto"]     = std::make_shared<SimpleToken>(LexerToken::AUTO);
    keywords["register"] = std::make_shared<SimpleToken>(LexerToken::REGISTER);
    keywords["typedef"]  = std::make_shared<SimpleToken>(LexerToken::TYPEDEF);
    keywords["return"]   = std::make_shared<SimpleToken>(LexerToken::RETURN);
}


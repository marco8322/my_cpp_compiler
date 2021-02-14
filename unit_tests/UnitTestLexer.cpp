// UnitTestLexer.cpp
//
// Unit tests for Lexer classes
//

#include "Lexer.hpp"
#include "UnitTest.hpp"
#include <initializer_list>

/**
 * My own char reader with a string
 */
class MyCharReader : public CharReader {
    std::string theString;
    std::string::iterator currChar;

public:
    MyCharReader(const std::string& theString_) : theString(theString_)
    {
        currChar = theString.begin();
    }

    virtual int getNextChar() 
    { 
        int result = peekNextChar();
        ++currChar;

        return result;
    }

    virtual int peekNextChar() 
    { 
        if( currChar != theString.end() )
        {
            return *currChar;
        } 
        else
        {
            return EOF;
        }
    }
};

/**
 * My own error messaging
 */

class MyMessage : public Message {
    Msg msgToSend;
    std::vector<std::string> msgArgs;

public:
    virtual void error(Msg msg, std::initializer_list<std::string> args) 
    { 
        msgToSend = msg;
        msgArgs.insert(msgArgs.begin(), args.begin(), args.end());
    }

    void resetError()
    {
        msgToSend = Message::NO_ERROR;
        msgArgs.clear();
    }

    bool anyError() 
    {
        return msgToSend != NO_ERROR;
    }

    Msg getMessage() { return msgToSend; }
    const std::vector<std::string>& getArgs() { return msgArgs; }
};

/**
 * Basic unit tests for testing the interface of the lexer
 */
void basicC90InterfaceTests()
{
    std::shared_ptr<CharReader> myReader = std::make_shared<MyCharReader>("int myId  char++");
    std::shared_ptr<MyMessage> myMessage = std::make_shared<MyMessage>();

    C90Lexer c90Lexer(myReader, myMessage);
    LexerTokenPtr token;

    // Check peek token.
    //
    myMessage->resetError();
    token = c90Lexer.peekToken();
    UnitTest::assertTrue("TestPeek1", token->getKind() == LexerToken::INT);
    UnitTest::assertFalse("TestMsg1", myMessage->anyError());

    // Check peek token again does not go to next one
    //
    token = c90Lexer.peekToken();
    UnitTest::assertTrue("TestPeek2", token->getKind() == LexerToken::INT);
    UnitTest::assertFalse("TestMsg2", myMessage->anyError());

    // Read the token
    //
    token = c90Lexer.nextToken();
    UnitTest::assertTrue("TestNext2", token->getKind() == LexerToken::INT);
    UnitTest::assertFalse("TestMsg3", myMessage->anyError());

    // Peek the 2nd token
    //
    token = c90Lexer.peekToken();
    UnitTest::assertTrue("TestPeek3", token->getKind() == LexerToken::IDENTIFIER);
    UnitTest::assertFalse("TestMsg4", myMessage->anyError());

    // Accept the token
    //
    token = c90Lexer.acceptToken(LexerToken::IDENTIFIER);
    UnitTest::assertTrue("TestAccept1", token->getKind() == LexerToken::IDENTIFIER);
    UnitTest::assertFalse("TestMsg5", myMessage->anyError());

    // Call accept and get a failure
    //
    token = c90Lexer.acceptToken(LexerToken::SHORT);
    UnitTest::assertTrue("TestAccept2", token == nullptr);
    UnitTest::assertTrue("TestMsg6", myMessage->anyError());
    UnitTest::assertTrue("TestMgs7", myMessage->getMessage() == Message::ERROR_EXPECTED_TOKEN);
    myMessage->resetError();

    token = c90Lexer.acceptToken(LexerToken::CHAR);
    UnitTest::assertTrue("TestPeek3", token->getKind() == LexerToken::CHAR);
    UnitTest::assertFalse("TestMsg8", myMessage->anyError());

    // Accept ++
    //
    token = c90Lexer.acceptToken(LexerToken::INCR);
    UnitTest::assertTrue("TestAccept9", token->getKind() == LexerToken::INCR);
    UnitTest::assertFalse("TestMsg9", myMessage->anyError());

    // Accept EOF
    //
    token = c90Lexer.acceptToken(LexerToken::END_OF_FILE);
    UnitTest::assertTrue("TestAccept9", token->getKind() == LexerToken::END_OF_FILE);
    UnitTest::assertFalse("TestMsg9", myMessage->anyError());

    // Accept EOF again
    //
    token = c90Lexer.acceptToken(LexerToken::END_OF_FILE);
    UnitTest::assertTrue("TestAccept10", token->getKind() == LexerToken::END_OF_FILE);
    UnitTest::assertFalse("TestMsg10", myMessage->anyError());
}

/**
 * Test keywords that may be part of types or qualifiers
 * 
 * void, char, short, int, long, float, double, const, volatile, struct, union, enum, typedef, auto, register, extern, static
 */
void testC90TypeKeywords()
{
    std::shared_ptr<CharReader> myReader = std::make_shared<MyCharReader>(
        "void char short int long float double const volatile struct union enum typedef auto register extern static"
        " signed unsigned"
        );

    std::shared_ptr<MyMessage> myMessage = std::make_shared<MyMessage>();
    C90Lexer c90Lexer(myReader, myMessage);

    UnitTest::assertTrue("Test void", c90Lexer.nextToken()->getKind() == LexerToken::VOID);

    UnitTest::assertTrue("Test char", c90Lexer.nextToken()->getKind() == LexerToken::CHAR);
    UnitTest::assertTrue("Test short", c90Lexer.nextToken()->getKind() == LexerToken::SHORT);
    UnitTest::assertTrue("Test int", c90Lexer.nextToken()->getKind() == LexerToken::INT);
    UnitTest::assertTrue("Test long", c90Lexer.nextToken()->getKind() == LexerToken::LONG);

    UnitTest::assertTrue("Test float", c90Lexer.nextToken()->getKind() == LexerToken::FLOAT);
    UnitTest::assertTrue("Test double", c90Lexer.nextToken()->getKind() == LexerToken::DOUBLE);

    UnitTest::assertTrue("Test const", c90Lexer.nextToken()->getKind() == LexerToken::CONST);
    UnitTest::assertTrue("Test volatile", c90Lexer.nextToken()->getKind() == LexerToken::VOLATILE);

    UnitTest::assertTrue("Test struct", c90Lexer.nextToken()->getKind() == LexerToken::STRUCT);
    UnitTest::assertTrue("Test union", c90Lexer.nextToken()->getKind() == LexerToken::UNION);
    UnitTest::assertTrue("Test enum", c90Lexer.nextToken()->getKind() == LexerToken::ENUM);
    UnitTest::assertTrue("Test typedef", c90Lexer.nextToken()->getKind() == LexerToken::TYPEDEF);

    UnitTest::assertTrue("Test auto", c90Lexer.nextToken()->getKind() == LexerToken::AUTO);
    UnitTest::assertTrue("Test register", c90Lexer.nextToken()->getKind() == LexerToken::REGISTER);

    UnitTest::assertTrue("Test extern", c90Lexer.nextToken()->getKind() == LexerToken::EXTERN);
    UnitTest::assertTrue("Test static", c90Lexer.nextToken()->getKind() == LexerToken::STATIC);

    UnitTest::assertTrue("Test signed", c90Lexer.nextToken()->getKind() == LexerToken::SIGNED);
    UnitTest::assertTrue("Test unsigned", c90Lexer.nextToken()->getKind() == LexerToken::UNSIGNED);
}


/**
 * Unit tests for other keywords
 * 
 *  if, else, while, for, do, goto, break, continue, switch, case, default, return, sizeof
 */
void testC90OtherKeywords()
{
    std::shared_ptr<CharReader> myReader = std::make_shared<MyCharReader>(
        "if else while for do goto break continue switch case default return sizeof"
        );

    std::shared_ptr<MyMessage> myMessage = std::make_shared<MyMessage>();
    C90Lexer c90Lexer(myReader, myMessage);

    UnitTest::assertTrue("Test if", c90Lexer.nextToken()->getKind() == LexerToken::IF);
    UnitTest::assertTrue("Test else", c90Lexer.nextToken()->getKind() == LexerToken::ELSE);
    UnitTest::assertTrue("Test while", c90Lexer.nextToken()->getKind() == LexerToken::WHILE);
    UnitTest::assertTrue("Test for", c90Lexer.nextToken()->getKind() == LexerToken::FOR);
    UnitTest::assertTrue("Test do", c90Lexer.nextToken()->getKind() == LexerToken::DO);
    UnitTest::assertTrue("Test goto", c90Lexer.nextToken()->getKind() == LexerToken::GOTO);
    UnitTest::assertTrue("Test break", c90Lexer.nextToken()->getKind() == LexerToken::BREAK);
    UnitTest::assertTrue("Test continue", c90Lexer.nextToken()->getKind() == LexerToken::CONTINUE);
    UnitTest::assertTrue("Test switch", c90Lexer.nextToken()->getKind() == LexerToken::SWITCH);
    UnitTest::assertTrue("Test case", c90Lexer.nextToken()->getKind() == LexerToken::CASE);
    UnitTest::assertTrue("Test default", c90Lexer.nextToken()->getKind() == LexerToken::DEFAULT);
    UnitTest::assertTrue("Test return", c90Lexer.nextToken()->getKind() == LexerToken::RETURN);
    UnitTest::assertTrue("Test sizeof", c90Lexer.nextToken()->getKind() == LexerToken::SIZEOF);
}

/**
 * Make lexer unit tests.  Pass a test name, the string to read, the list of assert strings + expected tokens
 */
UnitTest::TestPtr makeLexerUnitTest(
    const std::string& testName,
    const std::string& charReaderStr,
    const std::initializer_list<std::string> &testStrs,
    const std::initializer_list<LexerToken::Kind> &expectedTokens
)
{
    if( testStrs.size() != expectedTokens.size() ) {
        std::cerr << "Error creating testName" << std::endl;
        std::exit(1);
    }

    std::vector<std::string> theTestStrs(testStrs);
    std::vector<LexerToken::Kind> theExpectedTokens(expectedTokens);

    auto func = [=](){
        std::shared_ptr<CharReader> myReader = std::make_shared<MyCharReader>(charReaderStr);
        std::shared_ptr<MyMessage> myMessage = std::make_shared<MyMessage>();
        C90Lexer c90Lexer(myReader, myMessage);

        auto testStrsIter = theTestStrs.begin();
        auto expectedTokensIter = theExpectedTokens.begin();

        while( testStrsIter != theTestStrs.end() ) {
            LexerTokenPtr nextToken = c90Lexer.nextToken();
            std::string nullPtrCheckStr(*testStrsIter);
            nullPtrCheckStr += " (null ptr)";
            UnitTest::assertTrue(nullPtrCheckStr, nextToken != nullptr);
            if( nextToken->getKind() != *expectedTokensIter) {
                std::cout << "Values: " << nextToken->getKind() << ", " << *expectedTokensIter << std::endl;
            }
            UnitTest::assertTrue(*testStrsIter, nextToken->getKind() == *expectedTokensIter);
            ++testStrsIter;
            ++expectedTokensIter;
        }
    };

    return UnitTest::makeSimpleTest(testName, func);
}

/**
 * Build the unit test object
 */
UnitTest::TestPtr buildLexerUnitTests()
{
    return UnitTest::makeMultipleTest(
        "All lexer tests",
        {
            UnitTest::makeSimpleTest("basicC90InterfaceTests", basicC90InterfaceTests),
            UnitTest::makeSimpleTest("testC90TypeKeywords", testC90TypeKeywords),
            UnitTest::makeSimpleTest("testC90OtherKeywords", testC90OtherKeywords),
            makeLexerUnitTest(
                "testC90AssignOps", 
                "= += -= *= /= %= &= |= ^= <<= >>=",
                {"Test =", "Test +=", "Test -=", "Test *=", "Test /=", "Test %=",
                    "Test &=", "Test |=", "Test ^=", "Test <<=", "Test >>="},
                {LexerToken::ASSIGN, LexerToken::ADD_ASSIGN, LexerToken::SUB_ASSIGN,
                    LexerToken::MUL_ASSIGN, LexerToken::DIV_ASSIGN,
                    LexerToken::MOD_ASSIGN, LexerToken::BIT_AND_ASSIGN,
                    LexerToken::BIT_IOR_ASSIGN, LexerToken::BIT_XOR_ASSIGN,
                    LexerToken::SHIFT_LEFT_ASSIGN, LexerToken::SHIFT_RIGHT_ASSIGN}
            ),
            makeLexerUnitTest(
                "testIncrDecr",
                "++ --",
                {"Test ++", "Test --"},
                {LexerToken::INCR, LexerToken::DECR}
            ),
            makeLexerUnitTest(
                "testC90ArithOps", 
                "+ - * / % & | ^ << >> ~",
                {"Test +", "Test -", "Test *", "Test /", "Test %",
                    "Test &", "Test |", "Test ^", "Test <<", "Test >>", "Test ~"},
                {LexerToken::ADD, LexerToken::SUB,
                    LexerToken::MUL, LexerToken::DIV,
                    LexerToken::MOD, LexerToken::BIT_AND,
                    LexerToken::BIT_IOR, LexerToken::BIT_XOR,
                    LexerToken::SHIFT_LEFT, LexerToken::SHIFT_RIGHT,
                    LexerToken::BIT_NOT}
            ),
            makeLexerUnitTest(
                "testC90BoolOps", 
                "&& || !",
                {"Test &&", "Test ||", "Test !"},
                {LexerToken::BOOL_AND, LexerToken::BOOL_OR, LexerToken::BOOL_NOT}
            ),
            makeLexerUnitTest(
                "testC90CompareOps",
                "< <= > >= == !=",
                {"Test <", "Test <=", "Test >", "Test >=", "Test ==", "Test !="},
                {LexerToken::LT, LexerToken::LE, LexerToken::GT,
                    LexerToken::GE, LexerToken::EQUAL, LexerToken::NOT_EQUAL}
            ),
            makeLexerUnitTest(
                "testC90MemberAccessOps",
                "[ ] -> .",
                {"Test [", "Test ]", "Test ->", "Test ."},
                {LexerToken::LEFT_BRACKET, LexerToken::RIGHT_BRACKET, 
                    LexerToken::LEFT_ARROW, LexerToken::DOT}
            ),
            makeLexerUnitTest(
                "testC90MemberOtherOps",
                "( ) , ? : ...",
                {"Test (", "Test )", "Test ,", "Test ?", "Test :", "Test ..."},
                {LexerToken::LEFT_PARAR, LexerToken::RIGHT_PARAR, 
                    LexerToken::COMMA, LexerToken::QUESTION_MARK,
                    LexerToken::COLON, LexerToken::DOT_DOT_DOT}
            )
        }
    );
}

/**
 * Just run the unit tests
 */
int main()
{
    return buildLexerUnitTests()->runTest();
}
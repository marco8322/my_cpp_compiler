#include "Lexer.hpp"

class MyCharReader : public CharReader {
public:
    virtual int getNextChar() { return -1; }
    virtual int peekNextChar() { return -1; }
};

class MyMessage : public Message {
public:
    virtual void error(Msg msg, std::initializer_list<std::string> args) { }
};

bool myTest()
{
    std::shared_ptr<CharReader> charReader = std::make_shared<MyCharReader>();
    std::shared_ptr<Message> msg = std::make_shared<MyMessage>();
    std::shared_ptr<Lexer> lexer = std::make_shared<C90Lexer>(charReader, msg);

    return true;
}

int main()
{
    return !myTest();
}
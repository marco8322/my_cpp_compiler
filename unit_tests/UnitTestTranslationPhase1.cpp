// UnitTestTranslationPhase1.cpp
//
// Author: Marco Jacques
//
// Unit tests for phase 1 of translation phase (replacing \r\n and trigraph sequences)
//

#include "C90Preprocess.hpp"
#include "UnitTest.hpp"

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


void testTranslateTrigraph()
{
    // Translate ??< -> }
    //
    auto filename = std::make_shared<std::string>("myfile.cpp");
    CharacterStream stream1("a ??""< b", filename, 1, 1);
    CharacterStreamList source;
    CharacterStreamList target;
    auto msg = std::make_shared<MyMessage>();

    source.push_back(stream1);

    PreprocessorPhase::convertNewlinesAndTrigraphs(source, target, msg);
    //std::cout << "Source[0]" << source[0].getStream() << std::endl;
    //std::cout << "Target[0]" << target[0].getStream() << std::endl;
    //std::cout << "Size = " << target.size() << std::endl;
    UnitTest::assertTrue("Test size", target.size() == 2);
    UnitTest::assertTrue("CheckStr1", target[0].getStream() == "a {");
    UnitTest::assertTrue("CheckStr2", target[1].getStream() == " b");
    UnitTest::assertTrue("CheckFile1", target[0].getFile() == filename);
    UnitTest::assertTrue("CheckFile2", target[1].getFile() == filename);
    UnitTest::assertTrue("CheckLine1", target[0].getStartLine() == 1);
    UnitTest::assertTrue("CheckLine2", target[1].getStartLine() == 1);
    UnitTest::assertTrue("CheckColumn1", target[0].getStartColumn() == 1);
    
    // NOTE: add assertEqual, with message: expecting, got...
    //
    UnitTest::assertTrue("CheckColumn2", target[1].getStartColumn() == 6);
}

UnitTest::TestPtr makeUnitTests()
{
    return UnitTest::makeSimpleTest("Test 1", testTranslateTrigraph);
}

int main()
{
    makeUnitTests()->runTest();
    return 0;
}

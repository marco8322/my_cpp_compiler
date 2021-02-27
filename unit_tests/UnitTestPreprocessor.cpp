// UnitTestTranslationPhase1.cpp
//
// Author: Marco Jacques
//
// Unit tests for phase 1 of translation phase (replacing \r\n and trigraph sequences)
//

#include "C90Preprocess.hpp"
#include "UnitTest.hpp"
#include "UnitTestMessage.hpp"


/**
 * Make a unit test for a trigraph translation
 */
UnitTest::TestPtr makeTrigraphSuccessTest(const char* testName, char origChar, char newChar)
{
    auto theFunc = [=](){

        // Translate ??'origChar' -> 'newChar'}
        //
        std::string originalString = std::string("a ??") + origChar + " b";
        std::string newExpectedString1 = std::string("a ") + newChar;
        std::string newExpectedString2 = " b";


        auto filename = std::make_shared<std::string>("myfile.cpp");
        CharacterStreamList source;
        CharacterStreamList target;
        auto msg = std::make_shared<UnitTestMessage>();

        source.push_back(CharacterStream(originalString, SourcePosition(filename, 1, 1)));

        PreprocessorPhases().convertNewlinesAndTrigraphs(source, target, msg);

        UnitTest::assertEquals("Test size", target.size(), 2);
        UnitTest::assertEquals("CheckStr1", target[0].getStream(), newExpectedString1);
        UnitTest::assertEquals("CheckStr2", target[1].getStream(), newExpectedString2);
        UnitTest::assertEquals("CheckFile1", target[0].getSourcePosition().getFilename(), filename);
        UnitTest::assertEquals("CheckFile2", target[1].getSourcePosition().getFilename(), filename);
        UnitTest::assertEquals("CheckLine1", target[0].getSourcePosition().getLineNumber(), 1);
        UnitTest::assertEquals("CheckLine2", target[1].getSourcePosition().getLineNumber(), 1);
        UnitTest::assertEquals("CheckColumn1", target[0].getSourcePosition().getColumnNumber(), 1);
        UnitTest::assertEquals("CheckColumn2", target[1].getSourcePosition().getColumnNumber(), 6);

        UnitTest::assertTrue("Check any error", msg->anyError());
        UnitTest::assertEquals("Check error code", msg->getMessage(), Message::WARNING_TRIGRAPH_REPLACED);
        UnitTest::assertEquals("Check error args", msg->getArgs(), std::vector<std::string>());
    };

    return UnitTest::makeSimpleTest(testName, theFunc);
}

/**
 * Unit test for replacing \r\n
 */
void testReplaceLineFeedNewlines()
{
    CharacterStreamList source;
    CharacterStreamList target;

    auto filename = std::make_shared<std::string>("myfile2.cpp");
    source.push_back(CharacterStream("a \r\n", SourcePosition(filename, 1, 1)));

    auto msg = std::make_shared<UnitTestMessage>();
    PreprocessorPhases().convertNewlinesAndTrigraphs(source, target, msg);

    UnitTest::assertEquals("Test size", target.size(), 1);
    UnitTest::assertEquals("CheckStr1", target[0].getStream(), "a \n");
    UnitTest::assertEquals("CheckFile1", target[0].getSourcePosition().getFilename(), filename);
    UnitTest::assertEquals("CheckLine1", target[0].getSourcePosition().getLineNumber(), 1);
    UnitTest::assertEquals("CheckColumn1", target[0].getSourcePosition().getColumnNumber(), 1);

    UnitTest::assertFalse("Check any error", msg->anyError());
}

/**
 * Make unit test for unrecognized characters
 */
UnitTest::TestPtr makeTestUnrecognizedCharacters(const char* testName, char theChar)
{
    auto theFunc = [=]() {
        std::string originalString = std::string("a ") + theChar + " b";
        std::string newExpectedString1 = std::string("a ");
        std::string newExpectedString2 = std::string(" b");

        auto filename = std::make_shared<std::string>("myfile3.cpp");
        CharacterStreamList source;
        CharacterStreamList target;
        auto msg = std::make_shared<UnitTestMessage>();

        source.push_back(CharacterStream(originalString, SourcePosition(filename, 1, 1)));
        PreprocessorPhases().convertNewlinesAndTrigraphs(source, target, msg);

        //std::cout << "target: " << target.size() << std::endl;

        UnitTest::assertEquals("Test size", target.size(), 2);
        UnitTest::assertEquals("CheckStr1", target[0].getStream(), "a ");
        UnitTest::assertEquals("CheckStr2", target[1].getStream(), " b");
        UnitTest::assertEquals("CheckFile1", target[0].getSourcePosition().getFilename(), filename);
        UnitTest::assertEquals("CheckFile2", target[1].getSourcePosition().getFilename(), filename);
        UnitTest::assertEquals("CheckLine1", target[0].getSourcePosition().getLineNumber(), 1);
        UnitTest::assertEquals("CheckLine2", target[1].getSourcePosition().getLineNumber(), 1);
        UnitTest::assertEquals("CheckColumn1", target[0].getSourcePosition().getColumnNumber(), 1);
        UnitTest::assertEquals("CheckColumn2", target[1].getSourcePosition().getColumnNumber(), 4);

        UnitTest::assertTrue("Check any error", msg->anyError());
        UnitTest::assertEquals("Check error code", msg->getMessage(), Message::ERROR_UNKNOWN_CHARACTER);
        UnitTest::assertEquals("Check error args", msg->getArgs(), std::vector<std::string>());
    };

    return UnitTest::makeSimpleTest(testName, theFunc);
}


/**
 * Unit test with multiple lines with ?? which are not trigraphs
 */
void testNonTrigraphs()
{
    CharacterStreamList source;
    CharacterStreamList target;

    auto filename = std::make_shared<std::string>("myfile2.cpp");
    source.push_back(CharacterStream("a ??? \n", SourcePosition(filename, 1, 1)));
    source.push_back(CharacterStream("b ??\r \n", SourcePosition(filename, 2, 1)));
    source.push_back(CharacterStream("c ??@ \n", SourcePosition(filename, 3, 1)));
    source.push_back(CharacterStream("d ??", SourcePosition(filename, 4, 1)));

    auto msg = std::make_shared<UnitTestMessage>();
    PreprocessorPhases().convertNewlinesAndTrigraphs(source, target, msg);

    UnitTest::assertEquals("Test size", target.size(), 5);
    UnitTest::assertEquals("CheckStr1", target[0].getStream(), "a ??? \n");
    UnitTest::assertEquals("CheckStr2", target[1].getStream(), "b ??");
    UnitTest::assertEquals("CheckStr3", target[2].getStream(), " \n");
    UnitTest::assertEquals("CheckStr4", target[3].getStream(), "c ??@ \n");
    UnitTest::assertEquals("CheckStr5", target[4].getStream(), "d ??");

    UnitTest::assertTrue("Check any error", msg->anyError());
}


/**
 * Make the unit tests for the phase 1 translation
 */
UnitTest::TestPtr makePhase1TranslationUnitTests()
{
    return UnitTest::makeMultipleTest(
        "Phase 1 translation unit tests",
        {
            UnitTest::makeSimpleTest("Test replace line feeds/newlines", testReplaceLineFeedNewlines),
            makeTestUnrecognizedCharacters("Test replace \\r", '\r'),
            makeTestUnrecognizedCharacters("Test replace \\177", '\177'),
            UnitTest::makeMultipleTest(
                "Test trigraphs", 
                {
                    makeTrigraphSuccessTest("Test < -> {", '<', '{'),
                    makeTrigraphSuccessTest("Test > -> }", '>', '}'),
                    makeTrigraphSuccessTest("Test ( -> [", '(', '['),
                    makeTrigraphSuccessTest("Test ) -> ]", ')', ']'),
                    makeTrigraphSuccessTest("Test = -> #", '=', '#'),
                    makeTrigraphSuccessTest("Test / -> \\", '/', '\\'),
                    makeTrigraphSuccessTest("Test ' -> ^", '\'', '^'),
                    makeTrigraphSuccessTest("Test ! -> |", '!', '|'),
                    makeTrigraphSuccessTest("Test - -> ~", '-', '~')
                }
            ),
            UnitTest::makeSimpleTest("Test non-trigraphs", testNonTrigraphs)
        }
    );
}

/**
 * Unit test for simple bachslash + newline replacement
 */
void testSimplePhase2()
{
    CharacterStreamList source;
    CharacterStreamList target;

    auto filename = std::make_shared<std::string>("myfile2.cpp");
    source.push_back(CharacterStream("a \n", SourcePosition(filename, 1, 1)));
    source.push_back(CharacterStream("b \\\n", SourcePosition(filename, 2, 1)));
    source.push_back(CharacterStream("c\n", SourcePosition(filename, 3, 1)));

    PreprocessorPhases().removeEndOfLineBacklashes(source, target);
    UnitTest::assertEquals("test size", target.size(), 3);
    UnitTest::assertEquals("test str1", target[0].getStream(), "a \n");
    UnitTest::assertEquals("test str2", target[1].getStream(), "b ");
    UnitTest::assertEquals("test str3", target[2].getStream(), "c\n");

    UnitTest::assertEquals("test file1", target[0].getSourcePosition().getFilename(), filename);
    UnitTest::assertEquals("test file2", target[1].getSourcePosition().getFilename(), filename);
    UnitTest::assertEquals("test file3", target[2].getSourcePosition().getFilename(), filename);

    UnitTest::assertEquals("test startline1", target[0].getSourcePosition().getLineNumber(), 1);
    UnitTest::assertEquals("test startline2", target[1].getSourcePosition().getLineNumber(), 2);
    UnitTest::assertEquals("test startline3", target[2].getSourcePosition().getLineNumber(), 3);

    UnitTest::assertEquals("test column1", target[0].getSourcePosition().getColumnNumber(), 1);    
    UnitTest::assertEquals("test column1", target[1].getSourcePosition().getColumnNumber(), 1);
    UnitTest::assertEquals("test column1", target[2].getSourcePosition().getColumnNumber(), 1);
}

/**
 * Unit test for simple bachslash + newline replacement
 */
void testPhase2CornerCases()
{
    CharacterStreamList source;
    CharacterStreamList target;

    auto filename = std::make_shared<std::string>("myfile2.cpp");
    source.push_back(CharacterStream("a \\", SourcePosition(filename, 1, 1)));
    source.push_back(CharacterStream("b \\", SourcePosition(filename, 2, 1)));
    source.push_back(CharacterStream("\n", SourcePosition(filename, 2, 4)));
    source.push_back(CharacterStream("c \\", SourcePosition(filename, 3, 1)));

    PreprocessorPhases().removeEndOfLineBacklashes(source, target);
    UnitTest::assertEquals("test size", target.size(), 3);
    UnitTest::assertEquals("test str1", target[0].getStream(), "a \\");
    UnitTest::assertEquals("test str2", target[1].getStream(), "b ");
    UnitTest::assertEquals("test str3", target[2].getStream(), "c \\");
}


/**
 * Unit test for phase 2 of translation
 */
UnitTest::TestPtr makePhase2UnitTests()
{
    return UnitTest::makeMultipleTest(
        "Phase 2 translation unit tests",
        {
            UnitTest::makeSimpleTest("Test simple", testSimplePhase2),
            UnitTest::makeSimpleTest("Test corner cases", testPhase2CornerCases)
        }
    );
}


/**
 * All unit tests for preprocessing
 */

UnitTest::TestPtr makePreprocessingUnitTests()
{
    return UnitTest::makeMultipleTest(
        "Preprocessor unit tests",
        {
            makePhase1TranslationUnitTests(),
            makePhase2UnitTests()
        }
    );    
}

/**
 * Run the unit tests
 */
int main()
{
    makePreprocessingUnitTests()->runTest();
    return 0;
}

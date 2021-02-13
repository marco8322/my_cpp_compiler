
#include "UnitTest.hpp"

void testFail()
{
    UnitTest::fail("call fail");
}

void testPassTrue()
{
    UnitTest::assertTrue("call pass 1", true);
}

void testFailTrue()
{
    UnitTest::assertTrue("call pass 2", false);
}

void testPassFalse()
{
    UnitTest::assertFalse("call false 1", false);
}

void testFailFalse()
{
    UnitTest::assertTrue("call false 2", false);
}

void testUncaughtException()
{
    throw 1;
}

UnitTest::TestPtr runOneTest()
{
    UnitTest::TestPtr aTest = UnitTest::makeSimpleTest("Test 1", testPassTrue);

    return aTest;
}

UnitTest::TestPtr runMultipleTests1()
{
    UnitTest::TestPtr theTests = UnitTest::makeMultipleTest(
        "My Mutiple Tests",
        {
            UnitTest::makeSimpleTest("Test 1", testFail),
            UnitTest::makeSimpleTest("Test 2", testPassTrue),
            UnitTest::makeSimpleTest("Test 3", testFailTrue),
            UnitTest::makeSimpleTest("Test 4", testPassFalse),
            UnitTest::makeSimpleTest("Test 5", testFailFalse),
            UnitTest::makeSimpleTest("Test 6", testUncaughtException)
        }
    );

    return theTests;
}

int main()
{
    UnitTest::TestPtr theTest;
    theTest = runMultipleTests1();
    theTest->runTest();

    return 0;
}

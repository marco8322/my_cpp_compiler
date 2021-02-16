// TestUnitTest.cpp
//
// Author: Marco Jacques
//
// Test the unit test utilities
//

#include "UnitTest.hpp"

/**
 * Test the 'fail' function
 */
void testFail()
{
    UnitTest::fail("call fail");
}

/**
 * Test the assertTrue function passing 'true'
 */
void testPassTrue()
{
    UnitTest::assertTrue("call pass 1", true);
}

/**
 * Test the assertTrue function passing 'false'
 */
void testFailTrue()
{
    UnitTest::assertTrue("call pass 2", false);
}

/**
 * Test the assertFalse function passing 'false'
 */
void testPassFalse()
{
    UnitTest::assertFalse("call false 1", false);
}

/**
 * Test the assertFalse function passing 'true'
 */
void testFailFalse()
{
    UnitTest::assertFalse("call false 2", true);
}

/**
 * Test when an uncaught exception
 */
void testUncaughtException()
{
    throw 1;
}

/**
 * Test make simple test with function pointer
 */
UnitTest::TestPtr runOneTestFunctionPointer()
{
    UnitTest::TestPtr aTest = UnitTest::makeSimpleTest("Test 1", testPassTrue);

    return aTest;
}

/**
 * Test make simple test with lambda
 */
UnitTest::TestPtr runOneTestLambda()
{
    UnitTest::UTFunc wrapper = [](){ testPassTrue(); };
    UnitTest::TestPtr aTest = UnitTest::makeSimpleTest("Test 1", wrapper);

    return aTest;
}

/**
 * Make a test instance with multiple tests
 */
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
            UnitTest::makeSimpleTest("Test 6", testUncaughtException),
            UnitTest::makeMultipleTest(
                "Multiple Tests 2",
                { runOneTestFunctionPointer(), runOneTestLambda() }
            )
        }
    );

    return theTests;
}

/**
 * Run the tests
 */
int main()
{
    UnitTest::TestPtr theTest;
    theTest = runMultipleTests1();
    return theTest->runTest() == 0;
}

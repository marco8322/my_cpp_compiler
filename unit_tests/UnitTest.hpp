// UnitTest.hpp
//
// Author: Marco Jacques
//
// Unit test utilities
//
#pragma once

#include <utility>
#include <string>
#include <iostream>
#include <initializer_list>
#include <memory>
#include <vector>
#include <functional>

namespace UnitTest {

    /**
     * Internal exception when a unit test fails
     */
    struct FailException {
        std::string msg;

        FailException(const std::string& msg_) : msg(msg_) { } 
    };

    /**
     * Fails unconditionally a unit test
     */
    void fail(const std::string& msg) 
    {
        throw FailException(msg);
    }

    /**
     * Check if a test is true. If not, the unit test fails
     */
    void assertTrue(const std::string& msg, bool condition)
    {
        if( !condition  ) throw FailException(msg);
    }

    /**
     * Check if a test is false.  If not, the unit test fails
     */
    void assertFalse(const std::string& msg, bool condition)
    {
        if( condition ) throw FailException(msg);
    }

    using UTFunc = std::function<void ()>;
    using UTResults = std::pair<int, int>;

    /**
     * Superclass for a tests.  This runs all tests, which should implement 'runTest' with argument
     */
    class Test {
    public:
        /**
         * Main function running the tests... print out the results
         */
        int runTest()
        {
            UTResults results = this->runTest(0);

            std::cout << std::endl << "Nb Pass: " << results.first << ", Nb fail: " << results.second << std::endl;

            return results.second;
        }

        /**
         * All test classes should implement this method
         */
        virtual UTResults runTest(int nbTabs) = 0;

        /**
         * Utility function for getting a string for tabs, dependent on the number of required tabs
         */
        inline static std::string getTabsStr(int nbTabs)
        {
            std::string result = "";
            while (--nbTabs >= 0) {
                result.push_back(' ');
                result.push_back(' ');
                result.push_back(' ');
                result.push_back(' ');
            }

            return result;
        }

        virtual ~Test() = default;
    };

    using TestPtr = std::shared_ptr<Test>;

    /**
     * This class is holding one unit test.
     */
    class SimpleTest : public Test {
    private:
        std::string testName;
        UTFunc utFunc;

    public:
        /**
         * Constructor
         */
        SimpleTest(const std::string& testName_, UTFunc utFunc_) : testName(testName_), utFunc(utFunc_) { }

        /**
         * Run the test...  print out if the test passes or fails.
         */
        virtual UTResults runTest(int nbTabs) 
        {
            try {
                // If not exception, the test succeed
                //
                utFunc();
                std::cout << getTabsStr(nbTabs) << "PASS (" << testName << ")" << std::endl;
                return std::make_pair<int, int>(1, 0);
            } catch (FailException e) {
                // If the unit test fail via our exception...
                //
                std::cout << getTabsStr(nbTabs) << "FAIL (" << testName << "): " << e.msg << std::endl;
                return std::make_pair<int, int>(0, 1);
            } catch (...) {
                // If the unit test fails because of another exception that is not caught...
                //
                std::cout << getTabsStr(nbTabs) << "FAIL (" << testName << "): " << "(uncaught exception)" << std::endl;
                return std::make_pair<int, int>(0, 1);
            }
        }
    };

    /**
     * Utility method for making a simple test.  This takes a std::function (lambda)
     */
    inline TestPtr makeSimpleTest(const std::string& testName, UTFunc utFunc)
    {
        return std::make_shared<SimpleTest>(testName, utFunc);
    }

    /**
     * Utility method for making a simple test.  This takes a function pointer and wraps the
     * call inside a lambda
     */
    inline TestPtr makeSimpleTest(const std::string& testName, void (*utFunc)())
    {
        auto wrapper = [=](){ utFunc(); };

        return makeSimpleTest(testName, wrapper);
    }

    using TestPtrList = std::initializer_list<TestPtr>;
    using TestPtrVector = std::vector<TestPtr>;

    /**
     * This holds a list of tests to run.  Each test by be a simple one, or hold multiple ones too 
     * (i.e. is a instance of this class)
     */
    class MultipleTest : public Test {
    private:
        std::string testName;
        TestPtrVector testList;

    public:
        /**
         * Constructor
         */
        MultipleTest(const std::string& testName_, TestPtrList testList_) : 
            testName(testName_), testList(testList_) 
        { 
        }

        /**
         *  Run the list of tests
         */
        virtual UTResults runTest(int nbTabs)
        {
            int pass = 0;
            int fail = 0;

            std::cout << getTabsStr(nbTabs) << "TEST SUITE (" << testName << ")" << std::endl;
            ++nbTabs;

            for( TestPtrVector::iterator testPtr = testList.begin();
                    testPtr != testList.end();
                    ++testPtr )
            {
                UTResults thisTestResults = (*testPtr)->runTest(nbTabs);
                pass += thisTestResults.first;
                fail += thisTestResults.second;
            }

            UTResults results = std::make_pair(pass, fail);
            return results;
        }
    };

    /**
     *  Utility function to make a test with a list of unit tests
     */
    inline TestPtr makeMultipleTest(const std::string& testName, const TestPtrList& testList)
    {
        return std::make_shared<MultipleTest>(testName, testList);
    }
}

#pragma once

#include <utility>
#include <string>
#include <iostream>
#include <initializer_list>
#include <memory>
#include <vector>
#include <functional>

namespace UnitTest {
    struct FailException {
        std::string msg;

        FailException(const std::string& msg_) : msg(msg_) { } 
    };

    void fail(const std::string& msg) 
    {
        throw FailException(msg);
    }

    void assertTrue(const std::string& msg, bool condition)
    {
        if( !condition  ) throw FailException(msg);
    }

    void assertFalse(const std::string& msg, bool condition)
    {
        if( condition ) throw FailException(msg);
    }

    using UTFunc = std::function<void ()>;
    using UTResults = std::pair<int, int>;

    class Test {
    public:
        int runTest()
        {
            UTResults results = this->runTest(0);

            std::cout << std::endl << "Nb Pass: " << results.first << ", Nb fail: " << results.second << std::endl;

            return results.second;
        }

        virtual UTResults runTest(int nbTabs) = 0;

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
    };

    using TestPtr = std::shared_ptr<Test>;

    class SimpleTest : public Test {
    private:
        std::string testName;
        UTFunc utFunc;

    public:
        SimpleTest(const std::string& testName_, UTFunc utFunc_) : testName(testName_), utFunc(utFunc_) { }

        virtual UTResults runTest(int nbTabs) 
        {
            try {
                utFunc();
                std::cout << getTabsStr(nbTabs) << "PASS (" << testName << ")" << std::endl;
                return std::make_pair<int, int>(1, 0);
            } catch (FailException e) {
                std::cout << getTabsStr(nbTabs) << "FAIL (" << testName << "): " << e.msg << std::endl;
                return std::make_pair<int, int>(0, 1);
            } catch (...) {
                std::cout << getTabsStr(nbTabs) << "FAIL (" << testName << "): " << "(uncaught exception)" << std::endl;
                return std::make_pair<int, int>(0, 1);
            }
        }
    };

    inline TestPtr makeSimpleTest(const std::string& testName, UTFunc utFunc)
    {
        return std::make_shared<SimpleTest>(testName, utFunc);
    }

    inline TestPtr makeSimpleTest(const std::string& testName, void (*utFunc)())
    {
        auto wrapper = [=](){ utFunc(); };

        return std::make_shared<SimpleTest>(testName, wrapper);
    }

    using TestPtrList = std::initializer_list<TestPtr>;
    using TestPtrVector = std::vector<TestPtr>;

    class MultipleTest : public Test {
    private:
        std::string testName;
        TestPtrVector testList;

    public:
        MultipleTest(const std::string& testName_, TestPtrList testList_) : 
            testName(testName_), testList(testList_) 
        { 
        }

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

    inline TestPtr makeMultipleTest(const std::string& testName, const TestPtrList& testList)
    {
        return std::make_shared<MultipleTest>(testName, testList);
    }
}
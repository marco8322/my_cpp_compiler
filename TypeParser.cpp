
#include "TypeParser.hpp"
#include <cassert>


/**
 * Constructor
 */
TypeNameResolver::TypeNameResolver(const std::shared_ptr<Message>& message_) :
        message(message_),
        typeFlags({false})
        {
            // Nothing else to do
        }

/**
 * This sets the corresponding flag.  If the flag was set already, we issue
 * en error. 
 * 
 * Returns true if no error was issued.
 */
bool TypeNameResolver::checkAndSetFlag(Flag flag)
{
    if( typeFlags[flag] ) {
        message->error(Message::ERROR_DUPLICATE_TYPE, {nameFlags[flag]});
        return false;
    }

    typeFlags[flag] = true;
    return true;
}

/**
 * Check if the flags is compatible with other set already.  If not, issue an
 * error (ex: void is not compatible with int)
 * 
 * Returns true if no error was issued
 */ 
bool TypeNameResolver::checkInvalidCombinations(Flag flag, std::initializer_list<Flag> flagsToCheck)
{
    assert(typeFlags[flag] == true);
    bool err = false;

    using Iter = std::initializer_list<Flag>::iterator;
    for( Iter currFlagToCheck = flagsToCheck.begin();
            currFlagToCheck != flagsToCheck.end();
            ++currFlagToCheck )
    {
        Flag flagToCheck = *currFlagToCheck;
        if( typeFlags[flagToCheck] == true ) {
            message->error(Message::ERROR_INVALID_TYPE_COMBO, {nameFlags[flag], nameFlags[flagToCheck]});
            err = true;
        }
    }

    return !err;
}

/**
 * Set a flag and check for errors
 * 
 * Return true if no erros were issued
 */
bool TypeNameResolver::setFlag(Flag flag) 
{
    switch(flag) {
        case VOID:
            return checkAndSetFlag(VOID) && 
                checkInvalidCombinations(VOID, {CHAR, SHORT, INT, LONG, FLOAT, DOUBLE});

        default:
            assert(0);
    }

    return false;
}


IRTypePtr C90TypeParser::typeName()
{
    return nullptr;
}

IRTypePtr C90TypeParser::typeSpecifier()
{
    return nullptr;
}
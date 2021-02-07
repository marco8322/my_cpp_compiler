#pragma once

#include "IR.hpp"
#include "Message.hpp"

class TypeParser {
public:
    virtual ~TypeParser() = 0;

    virtual IRTypePtr typeName() = 0;
};


class TypeNameResolver {
    std::shared_ptr<Message> message;

public:
    enum Flag {
        CHAR, SHORT, INT, LONG,
        VOID,
        FLOAT, DOUBLE,
        CONST, VOLATILE,
        NB_FLAGS
    };

    /**
     * Constructor
     */
    TypeNameResolver(const std::shared_ptr<Message>& message_);

    /**
     * This sets the corresponding flag.  If the flag was set already, we issue
     * en error. 
     * 
     * Returns true if no error was issued.
     */
    bool checkAndSetFlag(Flag flag);

    /**
     * Check if the flags is compatible with other set already.  If not, issue an
     * error (ex: void is not compatible with int)
     * 
     * Returns true if no error was issued
     */ 
    bool checkInvalidCombinations(Flag flag, std::initializer_list<Flag> flagsToCheck);

    /**
     * Set a flag.  
     * 
     * Returns true if no error was issued
     */
    bool setFlag(Flag flag);

private:
    bool typeFlags[NB_FLAGS];
    const char *const nameFlags[NB_FLAGS] = {
        "char", "short", "int", "long", 
        "void", 
        "float", "double", 
        "const", "volatile"
        };

};


class C90TypeParser : public TypeParser {
protected:
    std::shared_ptr<Lexer> lexer;
    std::shared_ptr<Message> message;

public:
    IRTypePtr typeName();
    
    IRTypePtr typeSpecifier();
};



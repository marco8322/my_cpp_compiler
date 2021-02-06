#pragma once

#include "IR.hpp"

class TypeParser {
public:
    virtual ~TypeParser() = 0;

    virtual IRTypePtr typeName() = 0;
};


class C90TypeParser : public TypeParser {
protected:
    std::shared_ptr<Lexer> lexer;
    
public:
    IRTypePtr typeName();
    
    IRTypePtr typeSpecifier();
};


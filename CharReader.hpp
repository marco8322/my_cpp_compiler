
#pragma once

class CharReader {
public:
    virtual int getNextChar() = 0;
    virtual int peekNextChar() = 0;
    virtual ~CharReader() = default;
};


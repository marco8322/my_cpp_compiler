
#pragma once

#include <initializer_list>
#include <string>

class Message {
public:
    enum Msg {
        NO_ERROR,
        ERROR_DUPLICATE_TYPE,
        ERROR_INVALID_TYPE_COMBO,
        ERROR_EXPECTED_TOKEN
    };

    virtual void error(Msg msg, std::initializer_list<std::string> args) = 0;
    virtual ~Message() = default;
};


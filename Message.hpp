
#pragma once

#include <initializer_list>
#include <string>

class Message {
public:
    enum Msg {
        ERROR_DUPLICATE_TYPE,
        ERROR_INVALID_TYPE_COMBO
    };

    virtual void error(Msg msg, std::initializer_list<std::string>> args);
};


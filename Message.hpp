// Message.hpp
//
// Author: Marco Jacques
//
// Definition for messaging
//

#pragma once

#include <initializer_list>
#include <string>
#include "SourcePosition.hpp"

class Message {
public:
    enum Msg {
        NO_ERROR,
        ERROR_DUPLICATE_TYPE,
        ERROR_INVALID_TYPE_COMBO,
        ERROR_EXPECTED_TOKEN,
        WARNING_TRIGRAPH_REPLACED,
        ERROR_UNKNOWN_CHARACTER
    };

    virtual void issueMessage(const SourcePosition& sourcePosition, Msg msg, std::initializer_list<std::string> args) = 0;
    virtual ~Message() = default;
};


// UnitTestMessage.hpp
//
// Implementation of message class for unit tests
//
// Author: Marco Jacques
//

#pragma once

/**
 * My own error messaging
 */

class UnitTestMessage : public Message {
    Msg msgToSend;
    std::vector<std::string> msgArgs;

public:
    virtual void issueMessage(Msg msg, std::initializer_list<std::string> args) 
    { 
        msgToSend = msg;
        msgArgs.insert(msgArgs.begin(), args.begin(), args.end());
    }

    void resetError()
    {
        msgToSend = Message::NO_ERROR;
        msgArgs.clear();
    }

    bool anyError() 
    {
        return msgToSend != NO_ERROR;
    }

    Msg getMessage() { return msgToSend; }
    const std::vector<std::string>& getArgs() { return msgArgs; }
};

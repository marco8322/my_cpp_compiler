
#pragma once

#include <memory>
#include <vector>
#include "Message.hpp"

class PPToken {

};

using PPTokenPtr = std::shared_ptr<PPToken>;
using PPTokenList = std::vector<PPTokenPtr>;

class PPLexer {

};

void preprocessorParser(
    const PPLexer& pplexer,
    const Message& msg,
    PPTokenList& tokenList
);



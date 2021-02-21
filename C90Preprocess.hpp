// CPreprocess.hpp
//
// Author: Marco Jacques
//
// Classes for C pre-processing
//
#pragma once

#include <string>
#include <memory>
#include <vector>
#include "Message.hpp"
#include "SourcePosition.hpp"

class CharacterStream {
private:
    std::string stream;
    SourcePosition sourcePosition;

public:
    CharacterStream(
        const std::string stream_, 
        SourcePosition sourcePosition_
        ) : stream(stream_), sourcePosition(sourcePosition_)
        {
            // nothing to do
        }

    const std::string& getStream() const { return stream; } 
    const SourcePosition& getSourcePosition() const { return sourcePosition; }
};

using CharacterStreamList = std::vector<CharacterStream>;

class Preprocessor {
public:
    virtual void doPreprocessor(const CharacterStreamList& input, CharacterStreamList& output) = 0;
};


class C90Preprocessor : public Preprocessor {
public:
    virtual void doPreprocessor(const CharacterStreamList& input, CharacterStreamList& output) override;
};

namespace PreprocessorPhase {

    /**
     * Phase 1 of translation: convert \r\n and trigraphs.  
     */

    void convertNewlinesAndTrigraphs(
        const CharacterStreamList& input, 
        CharacterStreamList& output,
        std::shared_ptr<Message> msg
    );    
    
    /**
     * Phase 2 of translation, remove all occurences of backslash + newline
     */
    void removeEndOfLineBacklashes(const CharacterStreamList& input, CharacterStreamList& output);

}


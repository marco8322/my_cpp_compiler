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

class CharacterStream {
private:
    std::string stream;
    std::shared_ptr<std::string> file;
    int startLine;
    int startColumn;

public:
    CharacterStream(
        const std::string stream_, 
        const std::shared_ptr<std::string> file_,
        int startLine_,
        int startColumn_
        ) : stream(stream_), file(file_), startLine(startLine_), startColumn(startColumn_)
        {
            // nothing to do
        }

    const std::string& getStream() const { return stream; } 
    const std::shared_ptr<std::string> getFile() const { return file; }
    int getStartLine() const { return startLine; }
    int getStartColumn() const { return startColumn; }
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
    void convertNewlinesAndTrigraphs(const CharacterStreamList& input, CharacterStreamList& output);
}


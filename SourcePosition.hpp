// SourcePosition.hpp
//
// Class to represent source positions
//
// Author: Marco Jacques
//

#pragma once

#include <memory>
#include <string>

/**
 * Class representing a source position
 */
class SourcePosition {
    std::shared_ptr<std::string> filename;
    int lineNumber;
    int columnNumber;

public:

    /**
     * Constructor
     */
    SourcePosition(
        const std::shared_ptr<std::string>& filename_,
        int lineNumber_,
        int columnNumber_
        ) :
        filename(filename_), lineNumber(lineNumber_), columnNumber(columnNumber_)
    {

    }

    /**
     * Getter functions
     */
    const std::shared_ptr<std::string>& getFilename() const { return filename; }
    int getLineNumber() const { return lineNumber; }
    int getColumnNumber() const { return columnNumber; }
};

/**
 * == operation between 2 source positions... mostly for unit testing
 */
inline bool operator==(const SourcePosition& pos1, const SourcePosition& pos2)
{
    if( pos1.getLineNumber() == pos2.getLineNumber() && pos1.getColumnNumber() == pos2.getColumnNumber() )
    {
        if( pos1.getFilename() != nullptr && pos2.getFilename() != nullptr ) {
            return (*pos1.getFilename() == *pos2.getFilename());
        }
    }
    
    return false;
}
// CPreprocess.hpp
//
// Author: Marco Jacques
//
// Classes for C pre-processing
//

#include "C90Preprocess.hpp"

namespace PreprocessorPhase {
    void convertNewlinesAndTrigraphs(const CharacterStreamList& input, CharacterStreamList& output)
    {
        // Loop on all lines, convert \r\n to \n, and convert trigraphs
        //
        for( auto streamListIter = input.begin(); streamListIter != input.end(); ++streamListIter ) {
            std::string currentCharStream;
            int currentColumn = streamListIter->getStartColumn();

            const std::string& currStreamStr = streamListIter->getStream();
            auto currCharPtr = currStreamStr.begin();

            while( currCharPtr != currStreamStr.end() ) {

                // If this is \r, we cut the line here and remove it
                //
                if( *currCharPtr == '\r' ) {
                    
                }
            }
        }
    }
}
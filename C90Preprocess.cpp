// CPreprocess.hpp
//
// Author: Marco Jacques
//
// Classes for C pre-processing
//

#include "C90Preprocess.hpp"

namespace PreprocessorPhase {

    bool checkIfTrigraphSequenceComing(
        std::string::const_iterator& currCharPtr,
        const std::string::const_iterator& endStr
    )
    {
        return ( *currCharPtr == '?' &&
                (currCharPtr + 1) != endStr && *(currCharPtr + 1) == '?' &&
                (currCharPtr + 2) != endStr
                );
    }

    bool replaceC90TrigraphSequences(
        std::string::const_iterator& currCharPtr,
        std::string& currentCharStream
    )
    {
        /**
         *  Primary	Trigraph
            {	??<
            }	??>
            [	??(
            ]	??)
            #	??=
            \	??/
            ^	??'
            |	??!
            ~	??-
         */
        switch( *(currCharPtr + 2) ) {
            case '<':
                currentCharStream.push_back('{');
                break;

            case '>':
                currentCharStream.push_back('{');
                break;

            case '(':
                currentCharStream.push_back('[');
                break;

            case ')':
                currentCharStream.push_back(']');
                break;

            case '=':
                currentCharStream.push_back('#');
                break;

            case '/':
                currentCharStream.push_back('\\');
                break;

            case '\'':
                currentCharStream.push_back('^');
                break;

            case '!':
                currentCharStream.push_back('|');
                break;

            case '-':
                currentCharStream.push_back('~');
                break;

            default:
                currentCharStream.push_back('?');
                currentCharStream.push_back('?');
                currentCharStream.push_back(*(currCharPtr+2));
                return false;
       }

       return true;
    }

    void convertNewlinesAndTrigraphs(
        const CharacterStreamList& input, 
        CharacterStreamList& output,
        std::shared_ptr<Message> msg
        )
    {
        // Loop on all lines, convert \r\n to \n, and convert trigraphs
        //
        for( auto streamListIter = input.begin(); streamListIter != input.end(); ++streamListIter ) {
            std::string currentCharStream;
            int currentColumn = streamListIter->getStartColumn();
            int startColumn = currentColumn;

            const std::string& currStreamStr = streamListIter->getStream();
            auto currCharPtr = currStreamStr.begin();

            while( currCharPtr != currStreamStr.end() ) {

                // If this is \r, we cut the line here and remove it
                //
                if( *currCharPtr == '\r' ) {
                    if( currCharPtr+1 != currStreamStr.end() && *(currCharPtr+1) == '\n' ) {
                        currentCharStream.push_back('\n');
                        ++currCharPtr;
                    }     
                    else {
                        // issue warning or error that we see a unknown character and skip it
                        //
                    }
                }

                // Trigraph translation... try to translate and remove it
                //
                else if( checkIfTrigraphSequenceComing(currCharPtr, currStreamStr.end())) {
                    currentColumn += 2;
                    if( replaceC90TrigraphSequences(currCharPtr, currentCharStream) ) {
                        output.push_back(CharacterStream(currentCharStream, streamListIter->getFile(), streamListIter->getStartLine(), startColumn));
                        startColumn = currentColumn + 1;
                        currentCharStream.clear();

                        // issue message for trigraph has been translated
                    }

                    std::advance(currCharPtr, 2);
                }

                // any other printable character...
                // 
                else if (isprint(*currCharPtr) || isspace(*currCharPtr) ) {
                    currentCharStream.push_back(*currCharPtr);
                } 
                else {
                    // issue warning for unprintable error
                }

                ++currCharPtr;
                ++currentColumn;
            }

            if( !currentCharStream.empty() ) {
                output.push_back(CharacterStream(currentCharStream, streamListIter->getFile(), streamListIter->getStartLine(), startColumn));
            }
        }
    }
}
// CPreprocess.hpp
//
// Author: Marco Jacques
//
// Classes for C pre-processing
//

#include "C90Preprocess.hpp"
#include <map>
#include <initializer_list>

namespace PreprocessorPhase {

    /**
     * Check if this is a potential trigraph sequence
     */
    static bool checkIfTrigraphSequenceComing(
        std::string::const_iterator& currCharPtr,
        const std::string::const_iterator& endStr
    )
    {
        return ( *currCharPtr == '?' &&
                (currCharPtr + 1) != endStr && *(currCharPtr + 1) == '?' &&
                (currCharPtr + 2) != endStr
                );
    }

    /**
     * Try to do a trigraph sequence.  Return true if a replacement has been done
     */
    bool replaceC90TrigraphSequences(
        std::string::const_iterator& currCharPtr,
        std::string& currentCharStream
    )
    {
        // Trigraph mappings ??<first> -> <second>
        //
        static std::map<char, char> charMappings(
            {
                std::pair<char, char>('<', '{'), std::pair<char, char>('>', '}'),
                std::pair<char, char>('(', '['), std::pair<char, char>(')', ']'),
                std::pair<char, char>('=', '#'), std::pair<char, char>('/', '\\'),
                std::pair<char, char>('\'', '^'), std::pair<char, char>('!', '|'),
                std::pair<char, char>('-', '~')
           }
        );

        auto iterNewChar = charMappings.find(*(currCharPtr+2));
        if( iterNewChar != charMappings.end() ) {
            currentCharStream.push_back(iterNewChar->second);
            return true;
        }
        else {
            currentCharStream.push_back('?');
            currentCharStream.push_back('?');
            currentCharStream.push_back(*(currCharPtr+2));    

            return false;
        }
    }

    /**
     * Convert \r\n replacements and trigraphs.  
     */
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
                if( *currCharPtr == '\r' && ( currCharPtr+1 != currStreamStr.end() && *(currCharPtr+1) == '\n' ) ) {
                    currentCharStream.push_back('\n');
                    ++currCharPtr;
                }

                // Trigraph translation... try to translate and remove it
                //
                else if( checkIfTrigraphSequenceComing(currCharPtr, currStreamStr.end())) {
                    if( replaceC90TrigraphSequences(currCharPtr, currentCharStream) ) {
                        output.push_back(CharacterStream(currentCharStream, streamListIter->getFile(), streamListIter->getStartLine(), startColumn));
                        currentColumn += 2;
                        startColumn = currentColumn + 1;
                        currentCharStream.clear();
                        std::advance(currCharPtr, 2);

                        // issue message for trigraph has been translated
                        //
                        msg->issueMessage(Message::WARNING_TRIGRAPH_REPLACED, {});
                    }
                    else {
                        // Advance only one '?'
                        //
                        currentCharStream.push_back('?');
                    }
                }

                // any other printable character...
                // 
                else if (isprint(*currCharPtr) || ( isspace(*currCharPtr) && *currCharPtr != '\r') ) {
                    currentCharStream.push_back(*currCharPtr);
                } 
                else {
                    // issue warning for unprintable error
                    //
                    msg->issueMessage(Message::ERROR_UNKNOWN_CHARACTER, {});
                    output.push_back(CharacterStream(currentCharStream, streamListIter->getFile(), streamListIter->getStartLine(), startColumn));
                    startColumn = currentColumn + 1;
                    currentCharStream.clear();
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
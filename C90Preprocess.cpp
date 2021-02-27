// CPreprocess.hpp
//
// Author: Marco Jacques
//
// Classes for C pre-processing
//

#include "C90Preprocess.hpp"
#include <map>
#include <initializer_list>

/**
 * Check if this is a potential trigraph sequence
 */
bool PreprocessorPhases::checkIfTrigraphSequenceComing(
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
bool PreprocessorPhases::replaceC90TrigraphSequences(
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
        return false;
    }
}

/**
 * Convert \r\n replacements and trigraphs.  
 */
void PreprocessorPhases::convertNewlinesAndTrigraphs(
    const CharacterStreamList& input, 
    CharacterStreamList& output,
    std::shared_ptr<Message> msg
    )
{
    // Loop on all lines, convert \r\n to \n, and convert trigraphs
    //
    for( auto streamListIter = input.begin(); streamListIter != input.end(); ++streamListIter ) {
        std::string currentCharStream;
        int currentColumn = streamListIter->getSourcePosition().getColumnNumber();
        int startColumn = currentColumn;

        const std::string& currStreamStr = streamListIter->getStream();
        auto currCharPtr = currStreamStr.begin();
        const SourcePosition& currSourcePosition = streamListIter->getSourcePosition();

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
                    output.push_back(CharacterStream(currentCharStream, SourcePosition(currSourcePosition.getFilename(), currSourcePosition.getLineNumber(), startColumn)));
                    currentColumn += 2;
                    startColumn = currentColumn + 1;
                    currentCharStream.clear();
                    std::advance(currCharPtr, 2);

                    // issue message for trigraph has been translated
                    //
                    msg->issueMessage(currSourcePosition, Message::WARNING_TRIGRAPH_REPLACED, {});
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
                msg->issueMessage(currSourcePosition, Message::ERROR_UNKNOWN_CHARACTER, {});
                output.push_back(CharacterStream(currentCharStream, SourcePosition(currSourcePosition.getFilename(), currSourcePosition.getLineNumber(), startColumn)));
                startColumn = currentColumn + 1;
                currentCharStream.clear();
            }

            ++currCharPtr;
            ++currentColumn;
        }

        if( !currentCharStream.empty() ) {
            output.push_back(CharacterStream(currentCharStream, SourcePosition(currSourcePosition.getFilename(), currSourcePosition.getLineNumber(), startColumn)));
        }
    }
}

/**
 * Phase 2 of translation, remove all occurences of backslash + newline
 */
void PreprocessorPhases::removeEndOfLineBacklashes(
    const CharacterStreamList& input, 
    CharacterStreamList& output
    )
{
    // Loop on all lines, convert \r\n to \n, and convert trigraphs
    //
    for( auto streamListIter = input.begin(); streamListIter != input.end(); ++streamListIter ) {

        auto currStr = streamListIter->getStream();
        int length = currStr.length();
        char lastChar = currStr[length-1];

        // If the stream ends with backslash + newline, remove those characters
        //
        if( lastChar == '\n' ) {
            if( length >= 2 && currStr[length-2] == '\\' ) {
                auto newStr = currStr.substr(0, length-2);
                output.push_back(CharacterStream(newStr, streamListIter->getSourcePosition()));
                continue;
            }
        }

        // If the stream ends with backslash, see if the next line is only the '\n', if so
        // we remove the backslash + and skip next stream
        //
        else if( lastChar == '\\' ) {
            auto nextStream = streamListIter + 1;
            if( nextStream != input.end() && nextStream->getStream() == "\n" ) {
                auto newStr = currStr.substr(0, length-1);
                output.push_back(CharacterStream(newStr, streamListIter->getSourcePosition()));
                ++streamListIter;  // skip over the stream with the newline to remove it
                continue;
            }
        }

        // Default behavior, keep the current stream
        //
        output.push_back(CharacterStream(*streamListIter));
    }
}



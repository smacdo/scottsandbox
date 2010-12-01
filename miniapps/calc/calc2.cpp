/**
 * Copyright (C) 2010 Scott MacDonald. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <cassert>
#include <cctype>

bool loadFile( const std::string& path, std::string& contents )
{
    std::ifstream file( path.c_str(), std::ios::in );

    if (! file )
    {
        return false;
    }

    //
    // Keep reading lines from the file until there are no more lines
    // to be read. Add each one to the vector
    //
    std::stringstream ss;
    std::string line;

    while ( file.good() )
    {
        std::getline( file, line );

        if ( line.size() > 0 )      // EOF after \n triggers blank line
        {
            ss << line << "\n";
        }
    }

    // Assign
    contents = ss.str();

    // Remove the trailing \n
    if ( contents.size() > 0 )
    {
        contents = contents.substr( 0, contents.size() - 1 );
    }

    // 
    // Clean up
    //
    file.close();
    return true;
}

class StringTokenizer
{
public:
    StringTokenizer( const std::string& input )
        : m_startpos( 0 ),
          m_endpos( input.size() ),
          m_errorpos( 0 ),
          m_input( input ),
          m_lastToken(),
          m_currentToken(),
          m_errorStr(),
          m_hasNextToken( true ),
          m_errorFlag( false )
    {
    }

    bool hasNext() const
    {
        assert (! (m_hasNextToken && m_errorFlag) );
        return m_hasNextToken;
    }

    bool hasError() const
    {
        return m_errorFlag;
    }

    size_t errorPos() const
    {
        return m_errorpos;
    }

    std::string errorString() const
    {
        return m_errorStr;
    }

    std::string nextToken()
    {
        assert (! (m_hasNextToken && m_errorFlag) );
        parseNext();
        return m_currentToken;
    }

private:
    void parseNext()
    {
        size_t parserCurrentPos = m_startpos;
        size_t cpos             = m_startpos;
        
        bool bStartedToken = false;
        size_t token_start = cpos;      // first char of token
        size_t token_end   = cpos;      // one past last char of token

        //
        // Don't parse on error
        //
        if ( m_errorFlag )
        {
            return;
        }

        //
        // Search for the next token. A token is defined to be
        //
        // \w*([A-Za-z0-9_]+)(\w|SOME_CONTROL_CHARACTER)
        while ( cpos < m_endpos )
        {
            cpos   = parserCurrentPos;
            char c = m_input[ parserCurrentPos++ ];

            //
            // Eat LHS whitespace, or exit. One of the two
            //
            if ( isspace( c ) )
            {
                // Is it on the left hand side or right hand size?
                if (! bStartedToken )
                {
                    // on the left hand side. eat it and continue
                    continue;
                }
                else
                {
                    // on the right hand side. Terminate the token
                    // search now.
                    break;
                }
            }

            //
            // Refuse to handle any non-printable characters
            //
            if (! isprint( c ) )
            {
                // Is this the end of character char?
                if ( c == '\0' )
                {
                    break;
                }
                
                // Nope, report an error
                std::cerr << "ENCOUNTERED INVALID CHAR: "
                          << (int) c
                          << std::endl;

                m_errorpos  = cpos;
                m_errorStr  = "Encountered unprintable ASCII character";
                m_errorFlag = true;
                break;
            }

            //
            // Detect and deal with () [] {}
            //
            if ( c == '(' || c == ')' ||
                 c == '[' || c == ']'    )
            {
                if ( bStartedToken )
                {
                    // End the token right now
                    break;
                }
                else
                {
                    assert( token_start == token_end );

                    // Create and end the token as being only this
                    // character
                    token_start = cpos;
                    token_end   = token_start + 1;
                    break;
                }
            }

            // Nope! This character belongs to the current token. We must
            // check if this is the first character in the token, or if the
            // token has already been started
            if (! bStartedToken )
            {
                // Token hasn't been started yet
                token_start   = cpos;
                token_end     = cpos;
                bStartedToken = true;
            }

            // Acknowledge that the token belongs (this will continue
            // to push token_end to be one past the actual last)
            token_end++;
        }

        //
        // Extract the token from the stream
        //
        if ( token_start == token_end )
        {
            // only way to trigger this is an error state
            assert( m_errorFlag );
            m_hasNextToken = false;
        }
        else
        {
            m_currentToken = m_input.substr(
                                     token_start,
                                     token_end - token_start
            );
        }

        if ( parserCurrentPos >= m_endpos )
        {
            m_hasNextToken = false;
        }

        // Update parser position
        m_startpos = parserCurrentPos;

        // Sanity checks
        assert (! (m_hasNextToken && m_errorFlag) );
    }

    size_t m_startpos;
    size_t m_endpos;
    size_t m_errorpos;
    std::string m_input;
    std::string m_lastToken;
    std::string m_currentToken;
    std::string m_errorStr;
    bool m_hasNextToken;
    bool m_errorFlag;
};


int main( int argc, char* argv[] )
{
    std::string contents;
    const std::string filename = "sample.calc";

    if (! loadFile( filename, contents ) )
    {
        std::cerr << "Failed top open input file" << std::endl;
        return 1;
    }

    StringTokenizer tokenizer( contents );

    while ( tokenizer.hasNext() )
    {
        std::string token = tokenizer.nextToken();
        std::cout << "TOKEN: '" << token << "'" << std::endl;
    }
}


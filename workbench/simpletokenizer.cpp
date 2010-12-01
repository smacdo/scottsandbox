/**
 * Copyright 2010 Scott MacDonald. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY SCOTT MACDONALD ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL <COPYRIGHT HOLDER> OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Scott MacDonald.
 */

/////////////////////////////////////////////////////////////////////////////
// HEADER
////////////////////////////////////////////////////////////////////////////
#ifndef SCOTT_WORKBENCH_SIMPLE_TOKENIZER_H
#define SCOTT_WORKBENCH_SIMPLE_TOKENIZER_H
#define SIMPLE_TOKENIZER_VERSION 2

#include <string>

class StringTokenizer
{
public:
    StringTokenizer( const std::string& input );

    bool hasNext() const;

    bool hasError() const;

    size_t errorPos() const;

    std::string errorString() const;

    std::string nextToken();

private:
    void parseNext();

private:
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

#endif

/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
#include <cassert>

#include <string>
#include <vector>
#include <sstream>
#include <cassert>
#include <cctype>

StringTokenizer::StringTokenizer( const std::string& input )
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

bool StringTokenizer::hasNext() const
{
    assert (! (m_hasNextToken && m_errorFlag) );
    return m_hasNextToken;
}

bool StringTokenizer::hasError() const
{
    return m_errorFlag;
}

size_t StringTokenizer::errorPos() const
{
    return m_errorpos;
}

std::string StringTokenizer::errorString() const
{
    return m_errorStr;
}

std::string StringTokenizer::nextToken()
{
    assert (! (m_hasNextToken && m_errorFlag) );
    parseNext();
    return m_currentToken;
}

void StringTokenizer::parseNext()
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
            
            m_errorpos  = cpos;
            m_errorStr  = "Encountered unprintable ASCII character";
            m_errorFlag = true;
            break;
        }

        //
        // If this isn't whitespace and this isn't part of the token
        // lexeme (has to be a letter, number or _) then we consider it
        // an operator character
        //
        if ( isalnum( c ) == false && c != '_' )
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

/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <googletest/googletest.h>

TEST(StringTokenizer,CreateItAndDoNothingOfUseWhatsoever)
{
    StringTokenizer st( "hallo" );
}

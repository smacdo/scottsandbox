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
#define SIMPLE_TOKENIZER_VERSION 3

#include <string>

/**
 * A very simple string tokenizer that transforms a string into a set
 * of tokens using built-in rules. Tokens are seperated from each other
 * by either whitespace or an operator character.
 *
 * \w*([A-Za-z0-9_]+)(\w|CONTROL_CHARS)
 */
class StringTokenizer
{
public:
    /**
     * Initialize the string tokenizer with an input string
     */
    StringTokenizer( const std::string& input );

    /**
     * Indicates if the tokenizer has more tokens to consume. This will
     * return false if the tokenizer is in an error state.
     */
    bool hasNext() const;

    /**
     * Indicates if the tokenizer is in an error state.
     */
    bool hasError() const;

    /**
     * Returns the first character position that caused the last error.
     * Not valid if the parser is not in an error state.
     */
    size_t errorPos() const;

    /**
     * Returns a human readable string that describes the error
     * Note valid if the parser is not in an error state.
     */
    std::string errorString() const;

    /**
     * Returns the next parsed token from the input string. If you
     * continue to request tokens from the tokenizer when there are none
     * left, the tokenizer will go into an error state and this method
     * will return empty strings.
     */
    std::string nextToken();

    /**
     * Returns the current parsed token from the input string
     */
    std::string currentToken() const;

private:
    /**
     * Internal method that contains the parsing logic. It will start
     * from the last point it left off, and create a new token. This will
     * set an error state if there are problems with parsing, or if the
     * user attempts to go beyond the end of the string.
     */
    void parseNext();

    /**
     * Internal helper method that allows the tokenizer to raise an
     * error condition. Once raised, the parser will no longer function
     * and it will report the error
     */
    void raiseError( size_t pos, const std::string& message );

    /**
     * Helper method used by the constructor. It will remove excess spaces
     * from both the beginning and the end of the input string.
     */
    std::string trimString( const std::string& input ) const;

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

#include <iostream>

StringTokenizer::StringTokenizer( const std::string& input )
    : m_startpos( 0 ),
      m_endpos( 0 ),
      m_errorpos( 0 ),
      m_input( trimString( input ) ),
      m_lastToken(),
      m_currentToken(),
      m_errorStr(),
      m_hasNextToken( true ),
      m_errorFlag( false )
{
    m_endpos = m_input.size();
}

bool StringTokenizer::hasNext() const
{
    assert (! (m_hasNextToken && m_errorFlag) );

    if ( hasError() )
    {
        return false;
    }
    else
    {
        return m_hasNextToken;
    }
}

bool StringTokenizer::hasError() const
{
    return m_errorFlag;
}

size_t StringTokenizer::errorPos() const
{
    if ( hasError() )
    {
        return m_errorpos;
    }
    else
    {
        // no error, no valid position
        return std::string::npos;
    }
}

std::string StringTokenizer::errorString() const
{
    if ( hasError() )
    {
        return m_errorStr;
    }
    else
    {
        // no error!
        return std::string();
    }
}

std::string StringTokenizer::currentToken() const
{
    return m_currentToken;
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
         
            raiseError( cpos,
                    "Encountered unprintable ASCII Char while parsing" );
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
    // Ensure we didn't go past the end of the string
    //
    if ( token_start == token_end )
    {
        // The parser tried was at the end when it asked to go to the next
        // token. Whoops!
        raiseError( token_start,
                    "Attempted to retrieve token beyond end of string" );
    }

    //
    // If there have been no errors, extract the next token
    //
    if (! hasError() )
    {
        assert( token_start < token_end );

        m_currentToken = m_input.substr(
                                    token_start,
                                    token_end - token_start
        );
    }
    else
    {
        // In the case of error, make the string be empty
        m_currentToken = std::string();
    }

    if ( parserCurrentPos >= m_endpos )
    {
        m_hasNextToken = false;
    }

    // Update parser position
    m_startpos = token_end;

    // Sanity checks
    assert (! (m_hasNextToken && m_errorFlag) );
}

void StringTokenizer::raiseError( size_t epos, const std::string& message )
{
    m_errorpos     = epos;
    m_errorStr     = message;
    m_errorFlag    = true;
    m_hasNextToken = false;
}

std::string StringTokenizer::trimString( const std::string& input ) const
{
    std::string::size_type left = 0, right = input.size();

    // left trim
    std::string::size_type a = input.find_first_not_of(" \t\r\n");
    left = ( a != std::string::npos ? a : input.size() );

    // right trim
    std::string::size_type b = input.find_last_not_of(" \t\r\n");
    right = ( b != std::string::npos ? b : 0 );

    // apply trim and return result
    return std::string( input, left, (right-left+1) );
}

/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <googletest/googletest.h>

TEST(StringTokenizer,CreateItAndDoNothingOfUseWhatsoever)
{
    StringTokenizer st( "hallo" );
    EXPECT_FALSE( st.hasError() );
}

TEST(StringTokenizer,TokenizeASingleWord)
{
    // init
    StringTokenizer st( "hallo" );
    EXPECT_FALSE( st.hasError() );
    EXPECT_TRUE( st.hasNext()   );

    // first and only token
    std::string token = st.nextToken();
    EXPECT_EQ( "hallo", token );
    EXPECT_FALSE( st.hasError() );
    EXPECT_FALSE( st.hasNext()  );
}

TEST(StringTokenizer,TokenizerTokenizesTwoWords)
{
    StringTokenizer st( "hello world" );
    std::string token;

    // first token
    token = st.nextToken();
    EXPECT_EQ( "hello", token );
    EXPECT_TRUE( st.hasNext() );
    EXPECT_FALSE( st.hasError() );

    // second token
    token = st.nextToken();
    EXPECT_EQ( "world", token );
    EXPECT_FALSE( st.hasNext() );
    EXPECT_FALSE( st.hasError() );
}

TEST(StringTokenizer,TokenizerWithSymbol)
{
    StringTokenizer st( "1 + 2");

    EXPECT_EQ( "1", st.nextToken() );
    EXPECT_EQ( "+", st.nextToken() );
    EXPECT_EQ( "2", st.nextToken() );

    EXPECT_FALSE( st.hasError() );
    EXPECT_FALSE( st.hasNext()  );
}

TEST(StringTokenizer,TokenizerWithTwoConsequtiveSymbols)
{
    StringTokenizer st("1 += 2");

    EXPECT_EQ( "1", st.nextToken() );
    EXPECT_EQ( "+", st.nextToken() );
    EXPECT_EQ( "=", st.nextToken() );
    EXPECT_EQ( "2", st.nextToken() );
}

TEST(StringTokenizer,TokenizerWithTwoSymbolsInterleaved)
{
    StringTokenizer st("mary = beth . jane");

    EXPECT_EQ( "mary", st.nextToken() );
    EXPECT_EQ( "=",    st.nextToken() );
    EXPECT_EQ( "beth", st.nextToken() );
    EXPECT_EQ( ".",    st.nextToken() );
    EXPECT_EQ( "jane", st.nextToken() );
}

TEST(StringTokenizer,TokenizesBasicExpression)
{
    StringTokenizer st( "apples = tree.apples:count * 22" );

    EXPECT_EQ( "apples", st.nextToken() );
    EXPECT_EQ( "=",      st.nextToken() );
    EXPECT_EQ( "tree",   st.nextToken() );
    EXPECT_EQ( ".",      st.nextToken() );
    EXPECT_EQ( "apples", st.nextToken() );
    EXPECT_EQ( ":",      st.nextToken() );
    EXPECT_EQ( "count",  st.nextToken() );
    EXPECT_EQ( "*",      st.nextToken() );
    EXPECT_EQ( "22",     st.nextToken() );

    EXPECT_FALSE( st.hasError() );
    EXPECT_FALSE( st.hasNext()  );
}

TEST(StringTokenizer,TokenizerWithEmptyString)
{
    StringTokenizer st( "    hello world    ");

    // first token
    EXPECT_EQ( "hello", st.nextToken() );

    // second token
    EXPECT_EQ( "world", st.nextToken() );

    // all done?
    EXPECT_FALSE( st.hasError() );
    EXPECT_FALSE( st.hasNext()  );
}

TEST(StringTokenizer,TokenizerGoPastLastTokenTriggersError)
{
    StringTokenizer st( "hello world" );

    EXPECT_EQ( "hello", st.nextToken() );
    EXPECT_EQ( "world", st.nextToken() );
    EXPECT_FALSE( st.hasError() );

    EXPECT_EQ( "",      st.nextToken() );
    EXPECT_TRUE( st.hasError() );
    EXPECT_FALSE( st.hasNext() );
    EXPECT_EQ( std::string(), st.currentToken() );
}


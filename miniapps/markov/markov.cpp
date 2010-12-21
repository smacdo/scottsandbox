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
#include <iostream>
#include <string>
#include <unordered_map>
#include <googletest/googletest.h>
#include <cassert>

///////////////////////////////////////////////////////////////////////////
// Simple debugging
///////////////////////////////////////////////////////////////////////////
struct nullstream : std::ostream
{
    struct nullbuf : std::streambuf
    {
        int overflow( int c )
        {
            return traits_type::not_eof(c);
        }
    } m_sbuf;

    nullstream()
        : std::ios( &m_sbuf ), std::ostream( &m_sbuf )
    {
    }
};

#define ERRORLOG getOutputLog(GErrors)
#define DEBUGLOG getOutputLog(GDebug)
#define TRACELOG getOutputLog(GTrace)
#define ENDLOG   << std::endl
bool GErrors = true;
bool GDebug  = false;
bool GTrace  = false;
nullstream GNullStream;

std::ostream& getOutputLog( bool returnOrNot )
{
    if ( returnOrNot )
    {
        return std::cerr;
    }
    else
    {
        return GNullStream;
    }
}

///////////////////////////////////////////////////////////////////////////
// Markov class
///////////////////////////////////////////////////////////////////////////
struct ElementNode;

typedef std::vector<std::string> MarkovChain;
typedef std::unordered_map<std::string, ElementNode*> ENodeChildren;
typedef ENodeChildren::iterator ENodeChildrenItr;
typedef ENodeChildren::const_iterator ENodeChildrenConstItr;

struct ElementNode
{
    ElementNode()
        : value(),
          weightsum(0),
          children()
    {
    }

    ElementNode( const std::string& value )
        : value( value ),
          weightsum( 0 ),
          children()
    {
    }

    ~ElementNode()
    {
        for ( ENodeChildrenItr itr  = children.begin();
                               itr != children.end();
                             ++itr )
        {
            delete itr->second;
        }
    }

    bool hasChildren() const
    {
        return children.size() > 0;
    }

    std::string value;
    size_t weightsum;
    ENodeChildren children;
};

class MarkovData
{
public:
    MarkovData( size_t depth )
        : m_depth( depth ),
          m_root()
    {
        assert( depth > 0 && depth < 10 );
    }

    int childrenAtRoot() const
    {
        return m_root.children.size();
    }

    int weightSumAtRoot() const
    {
        return m_root.weightsum;
    }

    void insert( const std::vector<std::string>& chain )
    {
        assert( chain.size() == m_depth+1 );

        //
        // Debug output
        //
        DEBUGLOG << "Inserting chain: " ENDLOG;
        for ( size_t i = 0; i < chain.size(); ++i )
        {
            DEBUGLOG << "\t'" << chain[i] << "'" ENDLOG;
        }

        //
        // Insert the chain into the markov database
        //
        ElementNode * node = &m_root;

        for ( size_t i = 0; i < chain.size(); ++i )
        {
            node = insertAt( node, chain[i] );
        }

        node->weightsum += 1;       // tick the leaf
    }

    ElementNode* getNodeFor( const MarkovChain& chain )
    {
        assert( chain.size() <= m_depth +1 );

        // Recursively look up the node
        return getNodeFor( chain, &m_root, 0 );
    }

    template<typename IteratorFunction>
    void iterateNodesInBFS( IteratorFunction& func ) const
    {
    }

protected:
    ElementNode* insertAt( ElementNode* node, const std::string& value )
    {
        assert( node != 0 );

        //
        // Test if the element exists as a child of the current node.
        // If it does, increment summation. Otherwise add the element as
        // a new child
        //
        ENodeChildrenItr pos   = node->children.find( value );
        ElementNode*     child = NULL;

        if ( pos == node->children.end() )
        {
            // element did not exist, so create a new element to store it
            child = new ElementNode( value );

            // add it
            node->children.insert( ENodeChildren::value_type(value, child) );

            // update parent summation as well
            node->weightsum += 1;

            // debug
            DEBUGLOG << "Inserting (CREATE) value: '" << value << "'" ENDLOG;
        }
        else
        {
            child = pos->second;

            // element exists
            node->weightsum += 1;
            
            // debug
            DEBUGLOG << "Inserting (EXIST) value: '" << value << "'" ENDLOG;
        }

        assert( child != 0 );
        assert( child != node );
        return child;
    }


    ElementNode* getNodeFor( const MarkovChain& chain,
                             ElementNode* node,
                             size_t depth )
    {
        ElementNode * result = NULL;

        if ( depth == chain.size() )
        {
            // This is the node to return
            result = node;
        }
        else
        {
            // We still haven't found the end of the chain. Keep
            // recursively looking for it (if it exists)
            const std::string& value = chain[depth];
            ENodeChildrenItr pos     = node->children.find( value );
            
            if ( pos != node->children.end() )
            {
                result = getNodeFor( chain, pos->second, depth+1 );
            }
        }

        return result;
    }

private:

    size_t      m_depth;
    ElementNode m_root;
};

std::ostream& operator << ( std::ostream& os, const MarkovChain& chain )
{
    os << "<" << chain.size() << ";";

    for ( size_t i = 0; i < chain.size(); ++i )
    {
        os << chain[i];

        if ( i < chain.size()-1 )
        {
            os << ",";
        }
    }

    os << ">";
    return os;
}

std::string toString( const std::vector<std::string>& chain )
{
    std::stringstream ss;
    ss << chain;
    return ss.str();
}

bool testSubchain( MarkovData& data, const MarkovChain& chain,
                   size_t weightsum,
                   size_t childcount )
{
    ElementNode * node = data.getNodeFor( chain );

    if ( node == NULL )
    {
        ERRORLOG << "Could not lookup chain: " << chain ENDLOG;
        return false;
    }

    if ( node->weightsum != weightsum )
    {
        ERRORLOG << "Chain had unexpected weight " << node->weightsum
                 << ": " << chain ENDLOG;
        return false;
    }

    if ( node->children.size() != childcount )
    {
        ERRORLOG << "Chain had unexpect child count "
                 << node->children.size()
                 << ": " << chain ENDLOG;
        return false;
    }

    return true;
}

bool isEqual( const MarkovChain& lhs,
              const MarkovChain& rhs ) 
{
    bool isEqual = true;

    // They are obviously not the same is they have different sizes
    if ( lhs.size() != rhs.size() )
    {
        isEqual = false;
    }

    // Make sure each element is the same
    for ( size_t i = 0; i < lhs.size() && isEqual; ++i )
    {
        if ( lhs[i] != rhs[i] )
        {
            isEqual = false;
        }
    }

    // If they are not equal, print them out for debugging
    if (! isEqual )
    {
        ERRORLOG << toString( lhs ) << " != " << toString( rhs ) ENDLOG;
    }
    
    return isEqual;
}

/////////////////////////////////////////////////////////////////////////////
// Unit tests
/////////////////////////////////////////////////////////////////////////////
#define MC MarkovChain

#define MC1(x)     MarkovChain({#x})
#define MC2(x,y)   MarkovChain({#x,#y})
#define MC3(x,y,z) MarkovChain({#x,#y,#z})

//===========================================================================
// Utility functions tests
//===========================================================================
TEST(MarkovTree,EmptyChainGivesEmptyChainString)
{
    MC a;
    EXPECT_EQ( "<0;>", toString( a ) );
}

TEST(MarkovTree,OneElementChainToString)
{
    EXPECT_EQ( "<1;apple>", toString( MC1(apple) ) );
}

TEST(MarkovTree,TwoElementChainToString)
{
    EXPECT_EQ( "<2;apple,peach>", toString( MC2(apple,peach)) );
}

TEST(MarkovTree,EmptyChainsAreEqual)
{
    EXPECT_TRUE( isEqual( MC(), MC() ) );
}

TEST(MarkovTree,OneLengthChainsAreEqual)
{
    EXPECT_TRUE( isEqual( MC1(bear), MC1(bear) ) );
}

TEST(MarkovTree,TwoLengthChainsAreEqual)
{
    EXPECT_TRUE( isEqual( MC2(nancy, katie), MC2(nancy,katie) ) );
}

TEST(MarkovTree,SubsetChainsAreNotEqual)
{
    EXPECT_FALSE( isEqual( MC3(nancy,katie,daisy),MC2(nancy,katie) ) );
}

TEST(MarkovTree,TwoLengthChainsCanBeNotEqual)
{
    EXPECT_FALSE( isEqual( MC2(bob,david), MC2(nancy,katie) ) );
}

//===========================================================================
// Element node tests
//===========================================================================
TEST(MarkovTree,BasicNodeCreation)
{
    ElementNode node("_root");

    EXPECT_EQ( node.value, "_root" );
    EXPECT_EQ( node.weightsum, (unsigned int) 0 );
    EXPECT_FALSE( node.hasChildren() );
}

//===========================================================================
// Basic markov data tests
//===========================================================================
TEST(MarkovTree,FindNonexistantChainReturnsNull)
{
    MarkovData data(2);

    data.insert( MC3(aa,bb,cc) );

    EXPECT_EQ( NULL, data.getNodeFor( MC1(xx) ) );
    EXPECT_EQ( NULL, data.getNodeFor( MC2(aa,cc) ) );
}

TEST(MarkovTree,InsertChainCanRetrieveIt)
{
    MarkovData data(2);

    data.insert( MC3(a,b,c) );
    ElementNode * n = NULL;

    // Lookup {"a"} --> {"b", "c"}
    EXPECT_TRUE( n != data.getNodeFor( MC1(a) ) );
}

TEST(MarkovTree,InsertChainCanRetrieveAllLevels)
{
    MarkovData data(2);

    data.insert( MC3(aa,bb,cc) );

    EXPECT_TRUE( testSubchain( data, MC1(aa),       1, 1 ) );
    EXPECT_TRUE( testSubchain( data, MC2(aa,bb),    1, 1 ) );
    EXPECT_TRUE( testSubchain( data, MC3(aa,bb,cc), 1, 0 ) );
}

TEST(MarkovTree,BasicOneLevelTreeSameWeights)
{
    MarkovData data(1);

    data.insert( MC2(a,b) );
    data.insert( MC2(b,c) );
    data.insert( MC2(c,d) );

    // make sure three nodes at root
    EXPECT_EQ( 3, data.childrenAtRoot() );

    // Verify subchains
    //  {a,b}
    EXPECT_TRUE( testSubchain(data, MC1(a),   1, 1) );
    EXPECT_TRUE( testSubchain(data, MC2(a,b), 1, 0) );

    // {b,c}
    EXPECT_TRUE( testSubchain(data, MC1(b),   1, 1) );
    EXPECT_TRUE( testSubchain(data, MC2(b,c), 1, 0) );

    // {c,d}
    EXPECT_TRUE( testSubchain(data, MC1(c),   1, 1) );
    EXPECT_TRUE( testSubchain(data, MC2(c,d), 1, 0) );
}

//===========================================================================
// Complex higher level tests of markov database
//===========================================================================
TEST(MarkovTree,ComplexOneLevelTreeTest)
{
    MarkovData data(1);

    // ab, bb, bc, cd, bc, ac, ab, ba, bb, bb
    data.insert( MC2(a,b) ); data.insert( MC2(b,b) );
    data.insert( MC2(b,c) ); data.insert( MC2(c,d) );
    data.insert( MC2(b,c) ); data.insert( MC2(a,c) );
    data.insert( MC2(a,b) ); data.insert( MC2(b,a) );
    data.insert( MC2(b,b) ); data.insert( MC2(b,b) );

    // make sure sum is 10 and 3 children
    EXPECT_EQ( 3, data.childrenAtRoot() );
    EXPECT_EQ( 10, data.weightSumAtRoot() );

    // Verify chains
    // a branch
    EXPECT_TRUE( testSubchain( data, MC1(a),   3, 2 ) );
    EXPECT_TRUE( testSubchain( data, MC2(a,b), 2, 0 ) );
    EXPECT_TRUE( testSubchain( data, MC2(a,c), 1, 0 ) );

    // b branch
    EXPECT_TRUE( testSubchain( data, MC1(b),   6, 3 ) );
    EXPECT_TRUE( testSubchain( data, MC2(b,a), 1, 0 ) );
    EXPECT_TRUE( testSubchain( data, MC2(b,b), 3, 0 ) );
    EXPECT_TRUE( testSubchain( data, MC2(b,c), 2, 0 ) );

    // c branch
    EXPECT_TRUE( testSubchain( data, MC1(c),   1, 1 ) );
    EXPECT_TRUE( testSubchain( data, MC2(c,d), 1, 0 ) );
}

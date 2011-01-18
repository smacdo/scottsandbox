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

#include "markov.h"
#include "markovdata.h"
#include "elementnode.h"
#include "markovfactory.h"
#include "logging.h"

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
    MC a = MarkovChain();
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

    EXPECT_TRUE( NULL == data.getNodeFor( MC1(xx) ) );
    EXPECT_TRUE( NULL == data.getNodeFor( MC2(aa,cc) ) );
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

    std::cout << data.debugDumpToString() << std::endl;
}

TEST(MarkovChain,ComplexTwoLevelTTreeest)
{
    MarkovData data(2);

    /**
     * (ROOT:24)
     *   - A:6
     *      - B:2
     *          - A:1
     *          - D:1
     *      - C:4
     *          - D:2
     *          - A:1
     *          - C:1
     *   - B:11
     *      - B:3
     *          - A:3
     *      - A:5
     *          - A:1
     *          - B:2
     *          - C:1
     *          - D:1
     *      - C:3
     *          - A:1
     *          - D:2
     *   - C:3
     *      - B:3
     *          - D:3
     *   - D:4
     *      - A:1
     *          - C:1
     *      - B:3
     *          - A:2
     *          - D:1
     */
    data.insert( MC3(a,b,a) ); data.insert( MC3(a,c,c) );   // 1
    data.insert( MC3(d,a,c) ); data.insert( MC3(c,b,d) );   // 2
    data.insert( MC3(a,c,d) ); data.insert( MC3(a,c,d) );   // 3
    data.insert( MC3(b,a,a) ); data.insert( MC3(b,b,a) );   // 4
    data.insert( MC3(b,c,a) ); data.insert( MC3(c,b,d) );   // 5
    data.insert( MC3(c,b,d) ); data.insert( MC3(a,b,d) );   // 6

    data.insert( MC3(a,c,a) ); data.insert( MC3(b,c,d) );   // 7
    data.insert( MC3(d,b,a) ); data.insert( MC3(b,c,d) );   // 8
    data.insert( MC3(d,b,a) ); data.insert( MC3(d,b,d) );   // 9
    data.insert( MC3(b,b,a) ); data.insert( MC3(b,a,c) );   // 10
    data.insert( MC3(b,a,b) ); data.insert( MC3(b,a,b) );   // 11
    data.insert( MC3(b,a,d) ); data.insert( MC3(b,b,a) );   // 12

    // make sure sum is 10 and 3 children
    EXPECT_EQ( 4, data.childrenAtRoot() );
    EXPECT_EQ( 24, data.weightSumAtRoot() );

    // Verify chains
    // a branch
    EXPECT_TRUE( testSubchain( data, MC1(a),   6, 2 ) );
    EXPECT_TRUE( testSubchain( data, MC2(a,b), 2, 2 ) );
    EXPECT_TRUE( testSubchain( data, MC2(a,c), 4, 3 ) );

    EXPECT_TRUE( testSubchain( data, MC3(a,b,a), 1, 0 ) );
    EXPECT_TRUE( testSubchain( data, MC3(a,b,d), 1, 0 ) );

    EXPECT_TRUE( testSubchain( data, MC3(a,c,d), 2, 0 ) );
    EXPECT_TRUE( testSubchain( data, MC3(a,c,a), 1, 0 ) );
    EXPECT_TRUE( testSubchain( data, MC3(a,c,c), 1, 0 ) );

    // b branch
    EXPECT_TRUE( testSubchain( data, MC1(b),  11, 3 ) );
    EXPECT_TRUE( testSubchain( data, MC2(b,a), 5, 4 ) );
    EXPECT_TRUE( testSubchain( data, MC2(b,b), 3, 1 ) );
    EXPECT_TRUE( testSubchain( data, MC2(b,c), 3, 2 ) );

    EXPECT_TRUE( testSubchain( data, MC3(b,b,a), 3, 0 ) );

    EXPECT_TRUE( testSubchain( data, MC3(b,a,a), 1, 0 ) );
    EXPECT_TRUE( testSubchain( data, MC3(b,a,b), 2, 0 ) );
    EXPECT_TRUE( testSubchain( data, MC3(b,a,c), 1, 0 ) );
    EXPECT_TRUE( testSubchain( data, MC3(b,a,d), 1, 0 ) );

    EXPECT_TRUE( testSubchain( data, MC3(b,c,a), 1, 0 ) );
    EXPECT_TRUE( testSubchain( data, MC3(b,c,d), 2, 0 ) );

    // c branch
    EXPECT_TRUE( testSubchain( data, MC1(c),   3, 1 ) );
    EXPECT_TRUE( testSubchain( data, MC2(c,b), 3, 1 ) );

    EXPECT_TRUE( testSubchain( data, MC3(c,b,d), 3, 0 ) );

    // d branch
    EXPECT_TRUE( testSubchain( data, MC1(d),   4, 2 ) );
    EXPECT_TRUE( testSubchain( data, MC2(d,a), 1, 1 ) );
    EXPECT_TRUE( testSubchain( data, MC2(d,b), 3, 2 ) );

    EXPECT_TRUE( testSubchain( data, MC3(d,a,c), 1, 0 ) );

    EXPECT_TRUE( testSubchain( data, MC3(d,b,a), 2, 0 ) );
    EXPECT_TRUE( testSubchain( data, MC3(d,b,d), 1, 0 ) );
}

TEST(MarkovChain, InsertBasicLetterString)
{
    MarkovData data = MarkovFactory::createFromLetters( 3, "ABCACBAD" );

    std::cout << data.debugDumpToString() << std::endl;

    // make sure it conforms to expectations
    EXPECT_TRUE( testSubchain( data, MC3(A,B,C), 1, 0 ) );
    EXPECT_TRUE( testSubchain( data, MC3(B,C,A), 1, 0 ) );
    EXPECT_TRUE( testSubchain( data, MC3(C,A,C), 1, 0 ) );
    EXPECT_TRUE( testSubchain( data, MC3(A,C,B), 1, 0 ) );
    EXPECT_TRUE( testSubchain( data, MC3(C,B,A), 1, 0 ) );
    EXPECT_TRUE( testSubchain( data, MC3(B,A,D), 1, 0 ) );
}

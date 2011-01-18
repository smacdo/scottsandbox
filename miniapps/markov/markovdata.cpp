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
#include <string>
#include <unordered_map>
#include <cassert>
#include <sstream>

#include "logging.h"
#include "markov.h"
#include "markovdata.h"
#include "elementnode.h"

MarkovData::MarkovData( size_t depth )
    : m_depth( depth ),
      m_root()
{
    assert( depth > 0 && depth < 10 );
}

int MarkovData::childrenAtRoot() const
{
    return m_root.children.size();
}

int MarkovData::weightSumAtRoot() const
{
    return m_root.weightsum;
}

void MarkovData::insert( const std::vector<std::string>& chain )
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

ElementNode* MarkovData::getNodeFor( const MarkovChain& chain )
{
    assert( chain.size() <= m_depth +1 );

    // Recursively look up the node
    return getNodeFor( chain, &m_root, 0 );
}

ElementNode* MarkovData::insertAt( ElementNode* node,
                                   const std::string& value )
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


ElementNode* MarkovData::getNodeFor( const MarkovChain& chain,
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

void debugDumpHelper( const ElementNode& e,
                      size_t indent,
                      std::ostringstream& str )
{
    ENodeChildrenConstItr itr = e.children.begin();

    // Print the node
    for ( size_t i = 0; i < indent; ++i ) { str << "\t"; }
    str << "- " << e.weightsum << ", " << e.children.size();
    str << ": " << e.value     << std::endl;

    // Print all of the children
    for ( ; itr != e.children.end(); ++itr )
    {
        debugDumpHelper( *itr->second, indent+1, str );
    }
}

std::string MarkovData::debugDumpToString() const
{
    std::ostringstream ss;
    debugDumpHelper( m_root, 0, ss );
    return ss.str();
}


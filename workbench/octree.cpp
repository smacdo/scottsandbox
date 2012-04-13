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
/////////////////////////////////////////////////////////////////////////////
#ifndef SCOTT_WORKBENCH_OCTREE_H
#define SCOTT_WORKBENCH_OCTREE_H
#define OCTREE_VERSION 2

#include <cstdlib>
#include <ostream>

#define OCTREE_DEBUG

const int OCTREE_CHILD_COUNT = 8;

struct Point
{
    Point( int x_, int y_, int z_ )
        : x(x_), y(y_), z(z_)
    {
    }

    int x;
    int y;
    int z;
};

std::ostream& operator << ( std::ostream& os, const Point& p )
{
    os << "<" << p.x << ", " << p.y << ", " << p.z << ">";
    return os;
}

/**
 * An octree node is a structure that stores position and value data in an
 * octree. This node struture is internal to the octree class, and should not
 * exposed to outside users.
 *
 * Additionally, be careful when destroying octree nodes. Currently the model
 * is for a node to delete all of its children, so if you are replacing a
 * node's data then it is prudent to overrite the data field.
 */
template<typename T>
struct TCubeGridNode
{
    TCubeGridNode()
        : children(NULL),
          data()
    {
    }

    TCubeGridNode( const T& d )
        : children(NULL),
          data(d)
    {
    }

    ~TCubeGridNode()
    {
        delete[] children;
        children = 0;
    }

    TCubeGridNode<T> * children;
    T data;

private:
    TCubeGridNode( const TCubeGridNode& rhs );
    TCubeGridNode& operator = ( const TCubeGridNode& rhs );
};

/**
 * 3D cube grid for storing generic data, using an octree implementation to
 * speed lookups.
 *
 * Height explanation:
 *   8^h => Number of total cubes storable by the octree
 *   2^h => Grid dimension for the octree (n x n x n)
 */
template<typename T>
class TCubeGrid
{
public:
    /**
     * Creates a new sparse cube grid capable of storing objects in a volume
     * of size dim x dim x dim.
     */
    TCubeGrid( size_t dim );

    /**
     * Destructor. Cleans up
     */
    ~TCubeGrid();

    /**
     * Returns the dimensions of the volume storing the cube. Since the
     * volume is a perfectly formed cube of (dim x dim x dim), we only need to
     * return dim.
     */
    size_t dim() const
    {
        return m_dim;
    }

    /**
     * Returns the height of the octree backing the grid
     */
    size_t treeHeight() const;

    /**
     * Returns the maximum number of object that the grid can accomodate
     */
    size_t maxCount() const;

    /**
     * Returns the number of octree nodes that have been created to store
     * all of the objects
     */
    size_t nodeCount() const;

    /**
     * Returns the number of objects that currently exist in the grid
     *   DOES NOT WORK CURRENTLY
     */
    size_t count() const;

    /**
     * Returns the amount of memory consumed by the octree
     */
    size_t memoryUsed() const;

    /**
     * Places an object at the requested point in the grid
     */
    void set( const T& value, const Point& pt );

    /**
     * Retrieves an object at the requested point in the grid
     */
    T get( const Point& pt ) const;

    /**
     * Tests if there is an object at the requested point in the grid
     */
    bool exists( const Point& pt ) const;

private:
    /**
     * Finds the octree node that is storing the requested object data. If
     * no such object was found, the flag createIfNotExist will determine
     * if the class will allocate space in the grid and return the allocated
     * node.
     */
    TCubeGridNode<T>* findCube( const Point& lookingFor,
                                  bool createIfNotExist ) const;

    /**
     * Instructs the requested node to return a count of the number of
     * children it has attached
     */
    size_t getChildNodeCount( const TCubeGridNode<T> * node ) const;

    /**
     * Sanity check that ensures a point is within valid bounds of the octree
     * This check is disabled when OCTREE_DEBUG is not defined
     */
    void checkPointBounds( const Point& pt ) const;

    /**
     * A bit of black magic trickery here to make offset look ups really
     * fast. To explain, each node has eight child nodes. The position of
     * each child is very specific...
     *
     * [document more]
     */
    int calcChildOffset( int x, int y, int z, int size ) const;

    size_t             m_dim;
    size_t             m_height;
    mutable size_t     m_nodeCount;
    size_t             m_objCount;
    TCubeGridNode<T> * m_root;
};

// #include <octree.inc> // brings in template implementation
#endif

/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
#include <cassert>
#include <cmath>
    
template<typename T>
TCubeGrid<T>::TCubeGrid( size_t dim )
    : m_dim( dim ),
      m_height( static_cast<size_t>( log(dim) / log(2.0) ) ),
      m_nodeCount( 0 ),
      m_objCount( 0 ),
      m_root( new TCubeGridNode<T>() )
{
    // Make sure dimensions were power of two
    assert( (dim & -dim ) == dim );
}

template<typename T>
TCubeGrid<T>::~TCubeGrid()
{
    delete m_root;
}

template<typename T>
size_t TCubeGrid<T>::treeHeight() const
{
    return m_height;
}

template<typename T>
size_t TCubeGrid<T>::memoryUsed() const
{
    return nodeCount() * sizeof(TCubeGridNode<T>);
}

template<typename T>
size_t TCubeGrid<T>::maxCount() const
{
    return pow( 8, m_dim );
}

template<typename T>
size_t TCubeGrid<T>::nodeCount() const
{
    return m_nodeCount;
}

template<typename T>
size_t TCubeGrid<T>::count() const
{
    return m_objCount;
}

template<typename T>
void TCubeGrid<T>::set( const T& value, const Point& pt )
{
    // sanity
    checkPointBounds( pt );

    // Get pointer to the cube that holds this point
    TCubeGridNode<T>* cube = findCube( pt, true );
    assert ( cube != NULL );

    // Assign it
    cube->data = value;
}

template<typename T>
T TCubeGrid<T>::get( const Point& pt ) const
{
    // sanity
    checkPointBounds( pt );

    // Get pointer to cube
    TCubeGridNode<T>* cube = findCube( pt, true );
    assert( cube != NULL );

    // Assign it
    return cube->data;
}

template<typename T>
bool TCubeGrid<T>::exists( const Point& pt ) const
{
    // sanity
    checkPointBounds( pt );

    // Does there exist a pointer to the requested cube?
    TCubeGridNode<T>* cube = findCube( pt, false );

    return ( cube != NULL );
}

/*template<typename T>
size_t TCubeGrid<T>::nodeCount() const
{
    return getChildNodeCount( m_root );
}*/

template<typename T>
TCubeGridNode<T>* TCubeGrid<T>::findCube( const Point& lookingFor,
                                          bool createIfNotExist ) const
{
    int csize               = m_height - 1;
    TCubeGridNode<T> * node = m_root;
//    bool bHadToCreate       = false;

    //
    // Search for the cube
    //
    while ( csize >= 0 )
    {
        // Does this node have children? If it doesn't, we'll either
        // need to return a failure, or create the child (if CINE is
        // set to true)
        if ( node->children == NULL && createIfNotExist )
        {
            node->children = new TCubeGridNode<T>[ OCTREE_CHILD_COUNT ];

            // Update the number of live nodes in octree
            m_nodeCount   += 8;
//            bHadToCreate   = true;
        }
        else if ( node->children == NULL )
        {
            return NULL;
        }

        //
        // Select the correct child, and continue searching until
        // we hit reach the bottom of the tree
        //
        int offset = calcChildOffset( lookingFor.x,
                                      lookingFor.y,
                                      lookingFor.z,
                                      csize );

        node = &node->children[0] + offset;
        csize--;
    }

    assert ( node != NULL );
    return node;
}

template<typename T>
size_t TCubeGrid<T>::getChildNodeCount( const TCubeGridNode<T> * node ) const
{
    size_t sum = 0;

    if ( node->children != NULL )
    {
        for ( int i = 0; i < OCTREE_CHILD_COUNT; ++i )
        {
            sum += getChildNodeCount( node->children + i ) + 1;
        }
    }

    return sum;
}

#ifdef OCTREE_DEBUG
template<typename T>
void TCubeGrid<T>::checkPointBounds( const Point& pt ) const
{
    assert( pt.x >= 0 && static_cast<size_t>(pt.x) < m_dim );
    assert( pt.y >= 0 && static_cast<size_t>(pt.y) < m_dim );
    assert( pt.z >= 0 && static_cast<size_t>(pt.z) < m_dim );
}
#else
template<typename T>
void TCubeGrid<T>::checkPointBounds( const Point& ) const
{
}
#endif

/**
  * A bit of black magic trickery here to make offset look ups really
  * fast. To explain, each node has eight child nodes. The position of
  * each child is very specific...
  *
  * [document more]
  */
template<typename T>
int TCubeGrid<T>::calcChildOffset( int x, int y, int z, int size ) const
{
    // [ 0: -x, -y, -z
    //   1:  x, -y, -z
    //   2: -x,  y, -z
    //   3:  x,  y, -z
    //   4: -x, -y,  z
    //   5:  x, -y,  z
    //   6: -x,  y,  z
    //   7:  x,  y,  z ]
    return ( ( (( z >> size ) & 1) << 2 ) |
           (   (( y >> size ) & 1) << 1 ) |
           (   (( x >> size ) & 1) << 0 ) );
}

/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cassert>
#include <cmath>

#include <googletest/googletest.h>

typedef TCubeGridNode<int> Node;
typedef TCubeGrid<int> Tree;

// todo
//   - implement a callback or some event when the node doesn't
//     exist (so the engine can generate new cubes)
//
//   - use smart pointer to hide cube data from engine. TCubeGrid returns
//     smart pointer that points directly to data stored in the cube.
//     Actually, how about forcing the engine to re-store a cube? That way
//     he can edit it on his own time, and maybe allow better threading
//
//   - LRU cache so we can quickly find the last N cubes without having
//     to look up cube constantly
//
//   - Test amount of time it takes to create and populate octree of height
//     6 and 8
//
//   - Test amount of time it takes to create and access octree of height
//     6 and 8
TEST(TCubeGrid,WorstCaseTest)
{
    const size_t dim      = 64;
    const size_t searches = 1000000;       // one million

    Tree tree( dim );
    int c = 0;
    int found = 0;

    for ( size_t z = 0; z < dim; ++z )
    {
        for ( size_t y = 0; y < dim; ++y )
        {
            for ( size_t x = 0; x < dim; ++x )
            {
                tree.set( c++, Point( x, y, z ) );
            }
        }
    }

    int height = (int) tree.treeHeight();

    for ( size_t i = 0; i < searches; ++i )
    {
        if ( tree.exists( Point( (i % dim), (i % (height/2)),
                                 (i % dim))))
        {
            found++;
        }
    }
}

TEST(TCubeGrid,TestCellsArePlacedCorrectly)
{
    const int height = 32;

    Tree tree( height );
    int c = 1;

    //
    // Set all values
    //
    for ( int z = 0; z < height; ++z )
    {
        for ( int y = 0; y < height; ++y )
        {
            for ( int x = 0; x < height; ++x )
            {
                tree.set( c++, Point( x, y, z ) );
            }
        }
    }

    c--;        // remove last value
    int ok = 0;

    // Test all values from top
    for ( int z = height-1; z >= 0; --z )
    {
        for ( int y = height-1; y >= 0; --y )
        {
            for ( int x = height-1; x >= 0; --x )
            {
                int val = tree.get( Point( x, y, z ) );

                EXPECT_EQ( c, val );

                if ( val != c )
                {
                    std::cerr << "failure at " << x << ", " << y
                              << ", " << z << ". Expected " << c
                              << " but got "
                              << tree.get( Point(x,y,z) ) << std::endl;
                    std::cerr << "but did get " << ok << std::endl;
                    return;
                }
                else
                {
                    ok++;
                }
                c--;
            }
        }
    }

    EXPECT_EQ( c, 0 );
}

///////////////////////////////////////////////////////////////////////////
// TCubeGrid - node tests
///////////////////////////////////////////////////////////////////////////
TEST(TCubeGrid,TCubeGridNodeCreatesNoChildrenByDefault)
{
    TCubeGridNode<int> on;
    EXPECT_EQ( 0u, on.children );
}

TEST(TCubeGrid,TCubeGridNodeAddData)
{
    TCubeGridNode<int> on( 42 );
    EXPECT_EQ( 42, on.data );
}

///////////////////////////////////////////////////////////////////////////
// TCubeGrid
///////////////////////////////////////////////////////////////////////////
TEST(TCubeGrid,TCubeGridHeight)
{
    const TCubeGrid<int> tree( 64 );
    EXPECT_EQ( (size_t) 6, tree.treeHeight() );
}

TEST(TCubeGrid,TCubeGridDimension)
{
    const TCubeGrid<int> tree( 64 );
    EXPECT_EQ( (size_t) 64, tree.dim() );
}

TEST(TCubeGrid,TCubeGridNodeCountIsCorrect)
{
    TCubeGrid<int> tree( 64 );

    tree.set( 2, Point( 0, 0, 0 ) );
    EXPECT_EQ( static_cast<size_t>(6 * 8), tree.nodeCount() );

    tree.set( 2, Point( 1, 0, 0 ) );
    EXPECT_EQ( static_cast<size_t>(6 * 8), tree.nodeCount() );

    tree.set( 2, Point( 63, 63, 63 ) );
    EXPECT_EQ( static_cast<size_t>(6 * 8 + 5 * 8), tree.nodeCount() );
}

TEST(TCubeGrid,TCubeGridBasicGetSet)
{
    TCubeGrid<int> tree( 64 );

    tree.set( 6, Point( 2, 4, 6 ) );
    EXPECT_EQ( 6, tree.get( Point( 2, 4, 6 ) ) );
}

TEST(TCubeGrid,TCubeGridCubeNotSetDoesNotExist)
{
    TCubeGrid<int> tree( 64 );

    EXPECT_FALSE( tree.exists( Point(0, 0, 0) ) );
}

TEST(TCubeGrid,TCubeGridCubeSetDoesExist)
{
    TCubeGrid<int> tree( 64 );

    tree.set( 42, Point(0, 0, 0) );
    EXPECT_TRUE( tree.exists( Point(0, 0, 0) ) );
}


#ifndef QUADTREE_H
#define QUADTREE_H

#include <cassert>
#include <cmath>
#include "core/util.h"

#include <stdio.h>

enum Direction
{
    DIR_DOWN,
    DIR_RIGHT,
    DIR_UP,
    DIR_LEFT
};

bool isPowerOfTwo( int n )
{
    return ( (n & (n-1)) == 0 );
}

template<typename T>
class QuadNode
{
public:
    QuadNode()
        : x(0.0f), y(0.0f), children(NULL), data()
    {
        printf("ctor1\n");
    }

    QuadNode( int x, int y )
        : x(x), y(y), children(NULL), data()
    {
        printf("ctor2 %d,%d\n", x, y);
        assert( x != 0 && y != 0 ); // never possible
    }

    QuadNode( int x, int y,
              QuadNode<T>* ll,
              QuadNode<T>* lr,
              QuadNode<T>* ur,
              QuadNode<T>* ul )
        : x(x), y(y), children(new QuadNode<T>[4]), data()
    {
        printf("ctor3 %d,%d\n", x, y);
        assert( ll != NULL || lr != NULL || ul != NULL || ur != NULL );
        int s = size() / 2;

        if ( ll != NULL )
        {
            children[0] = ll;
        }
        else
        {
            children[0].x = x - s;
            children[0].y = y - s;
        }

        if ( lr != NULL )
        {
            children[1] = lr;
        }
        else
        {
            children[1].x = x + s;
            children[1].y = y - s;
        }

        if ( ul != NULL )
        {
            children[2] = ur;
        }
        else
        {
            children[2].x = x + s;
            children[2].y = y + s;
        }

        if ( ur != NULL )
        {
            children[3] = ul;
        }
        else
        {
            children[3].x = x - s;
            children[3].y = y + s;
        }

        // assert the sizes of our children words
    }

    QuadNode( const QuadNode<T>& n )
    {
        printf("copyctor\n");
    }

    ~QuadNode()
    {
        printf("~dtor %d,%d,%s\n", x, y, children==NULL?"F":"T");
        delete[] children;
    }

    QuadNode<T>& operator = ( QuadNode<T>& rhs )
    {
        printf("=&\n");
    }

    QuadNode<T>& operator = ( QuadNode<T> * rhs )
    {
        printf("=*\n");

        assert( rhs != NULL );
        delete[] children;

        x = rhs->x;
        y = rhs->y;
        children = rhs->children;
    
        return *this;
    }

    int size() const
    {
        return powl( 2, firstSetBitPosition(x) );
    }

    bool hasChildren() const
    {
        return children != NULL;
    }

    int childCount() const
    {
        return ( hasChildren() ? 4 : 0 );
    }

    bool isInNode( int x, int y )
    {
        int s = size();

        return ( x >= this->x - s && x < this->x + s &&
                 y >= this->y - s && y < this->y + s );
    }

    QuadNode<T> * child( int i )
    {
        assert( i >= 0 && i <= 4 );
        assert( children != NULL );

        return children + i;
    }

    /*** may want to remove? **/
    void split()
    {
        assert( children == NULL );
        assert( size() > 1 );

        children = new QuadNode<T>[4];
        int half = size() / 2;

        children[0].x = x - half; children[0].y = y - half;
        children[1].x = x + half; children[1].y = y - half;
        children[2].x = x + half; children[2].y = y + half;
        children[3].x = x - half; children[3].y = y + half;

        assert( children != NULL );
    }

    int x, y;
    QuadNode<T>* children;
    T data;
};

/**
 *                         (2L, 2L)
 * +----------+----------+
 * |          |          |
 * |          |          |
 * |          |          |
 * +----------+----------|
 * |          |          |
 * |          |          |
 * |          |          |
 * +----------+----------+
 */
template<typename T>
class QuadTree
{
public:
    QuadTree<T>( int length )
        : m_root( new QuadNode<T>( length, length ) ),
          m_length( length )
    {
        printf("qt ctor %d\n", length);
        assert( length > 0 );
        assert( length == 1 || isPowerOfTwo( length ) );
    }

    QuadTree<T>( const QuadTree<T>& qt )
    {
        assert( false && "Not supported yet" );
    }

    ~QuadTree()
    {
        printf("qt dtor %d length\n", m_length);
        delete m_root;
        m_root = NULL;
    }

    QuadTree<T>& operator = ( const QuadTree<T>& rhs )
    {
        printf("qt = (currently %d, copying %d)\n", m_length, rhs.m_length );
        delete m_root;

        assert( m_root != NULL );
        assert( m_length > 0 && m_length < 1024 );

        m_root   = rhs.m_root;
        m_length = rhs.m_length;
    }

    int length() const
    {
        return m_length;
    }

    int maxNodeDepth() const
    {
        // dont use cache
        return static_cast<int>( logf(m_length) / logf(2) ) + 1;
    }

    // grow the universe
    bool growQuadTreeBounds()
    {
        assert( m_root != NULL );

        // Create a new node to hold our current world, and assign
        // the lower-left leaf to our current root
        QuadNode<T> * root = new QuadNode<T>( 
                                    m_length * 2, m_length * 2,
                                    m_root, 
                                    NULL,
                                    NULL,
                                    NULL
        );

        m_root = root;

        return true;
    }

    QuadNode<T> * root()
    {
        return m_root;
    }

    QuadNode<T> * select( int x, int y )
    {
        assert( m_root != NULL );

        return select( m_root, x, y );
    }

    QuadNode<T> * select( QuadNode<T> * node, int x, int y )
    {
        assert( node != NULL );
        QuadNode<T> * result = NULL;

        //
        // Find the smallest node that can hold (x,y)
        //
        if ( node->hasChildren() )
        {
            for ( int i = 0; i < 4; ++i )
            {
                if ( node->children[i].isInNode( x, y ) )
                {
                    result = select( node->children + i, x, y );
                    break;
                }
            }
        }
        else if ( node->isInNode( x, y ) )
        {
            result = node;
        }

        return result;
    }

private:
    QuadNode<T> * m_root;
    int           m_length;
};

#endif

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

class Tilemap;
const size_t NODE_COUNT = 9;

struct Point
{
    Point( int x_, int y_ )
        : x(x_), y(y_)
    {
    }

    int x;
    int y;
};

struct PathNode
{
    Point pos;
    size_t cost;
};

class PathFinder
{
public:
    PathFinder( const Tilemap& map, const Point& start, const Point& end )
        : mMap( map ),
          mStart( start ),
          mEnd( end )
    {
        mOpenTiles.push_back( start );
    }

    std::vector<Point> findPath()
    {
        bool didFindAPath = false;
        std::vector<Point> neighbors;

        // keep searching until we find the destination, or until we 
        // fail to path to the destination
        while ( didFindAPath )
        {
            // Get the lowest cost open tile
            Point point = getLowestCost( mOpenTiles );

            // Make sure it gets added to the list of already visited tiles
            // so we don't path across this tile again
            mCloseTiles.push_back( point );

            // Who will be our neighbor?
            generateNeighbors( point, neighbors );

            // Calculate the cost of each neighbor, and so long as its
            // greater than one and hasn't already been considered push
            // it into the list of open tiles
            for ( size_t i = 0; i < neighbors.size(); ++i )
            {
                const Point& neighborPt = neighbors[i];
                int pointCost           = findCost( neighborPt) ;

                if ( neighborPt == point )
                {
                    // we found the destination!
                }
                else if ( false = wasAlreadyVisited( neighborPt ) )
                {
                    mOpenTiles.push_back( PathNode( neighborPt,
                                                    pointCost ) );
                }
            }

            // Clear out the list of neighbor tiles before considering
            // the next open square
            neighbors.clear();
        }

        // Grab the next tile
        Point pt = getNextOpenTile();
    }


private:
    int findCost( const Point& pt ) const
    {
        if ( pt.x < 0 || pt.x >= mWidth ||
             pt.y < 0 || pt.y >= mHeight )
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }

    Point getLowestPoint( std::vector<Point>& elements ) const
    {
        
    }

    void generateNeighbors( const Point& pt,
                            std::vector<Point>& candidates ) const
    {
        // north
        if ( pt.y > 0 )
        {
            candidates.push_back( Point( pt.x, pt.y - 1 ) );
        }

        // east
        if ( pt.x + 1 < mWidth )
        {
            candidates.push_back( Point( pt.x + 1, pt.y ) );
        }

        // south
        if ( pt.y + 1 < mHeight )
        {
            candidates.push_back( Point( pt.x, pt.y + 1 ) );
        }

        // north
        if ( pt.x > 0 )
        {
            candidates.push_back( Point( pt.x - 1, pt.y ) );
        }
    }

private:
    const Tilemap& mMap;
    const Point& mStart;
    const Point& mEnd;
    std::vector<Point> mOpenTiles;
    std::vector<Point> mClosedTiles;
};

void carvePath( const Point& start, const Point& end )
{
//    assert( fromId < NODE_COUNT );
//    assert( toId < NODE_COUNT );

    // Data
 //   bool didFindPath = false;
//    std::vector<Point> openSquares;
//    std::vector<Point> closedSquares;

    // Create points
//    Point startPt( GNodes[fromId][0], GNodes[fromId][1] );
//    Point endPt(   GNodes[toId][0],   GNodes[toId][1] );

    // Insert the starting point before starting the pathfinding
//    openSquares.push_back( startPt );

    // Now perform the path finding operation
  //  while ( didFindPath == false )
  //  {
  //  }
}

class Tilemap
{
public:
    Tilemap( size_t width, size_t height )
        : mWidth( width ),
          mHeight( height ),
          mTiles( height * width )
    {
        fill( '.' );
    }

    void print() const
    {
        for ( size_t y = 0; y < mHeight; ++y )
        {
            for ( size_t x = 0; x < mWidth; ++x )
            {
                std::cout << get( Point(x,y) );
            }

            std::cout << std::endl;
        }
    }

    void set( const Point& p, char c )
    {
        assert( p.x < mWidth && p.y < mHeight );
        mTiles[ p.y * mWidth + p.x ] = c;
    }

    char get( const Point& p ) const
    {
        assert( p.x < mWidth && p.y < mHeight );
        return mTiles[ p.y * mWidth + p.x ];
    }

    void fill( char c )
    {
        for ( size_t y = 0; y < mHeight; ++y )
        {
            for ( size_t x = 0; x < mWidth; ++x )
            {
                set( Point(x, y), c );
            }
        }
    }

private:
    size_t mWidth;
    size_t mHeight;
    std::vector<char> mTiles;
};

int main( int argc, char* argv[] )
{
    Tilemap map( 78, 20 );
    carvePath( Point( 5, 10 ), Point( 60, 15 ) );

    map.print();

}

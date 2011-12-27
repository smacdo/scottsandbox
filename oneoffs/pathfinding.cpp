#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>

#include <queue>

class Tilemap;
const size_t NODE_COUNT = 9;
const int MOVEMENT_COST = 2;

struct Point
{
    Point( int x_, int y_ )
        : x(x_), y(y_)
    {
    }

    bool operator == ( const Point& rhs ) const
    {
        return x == rhs.x && y == rhs.y;
    }

    bool operator != ( const Point& rhs ) const
    {
        return x != rhs.x || y != rhs.y;
    }

    int x;
    int y;
};

std::ostream& operator << ( std::ostream& ss, const Point& p )
{
    ss << "<" << p.x << ", " << p.y << ">";
    return ss;
}

struct PathNode
{
public:
    PathNode()
        : mPrevPos( 0, 0 ),
          mMovementCost( 0 ),
          mEstimatedCost( 0 )
    {
    }
        
    bool operator < ( const PathNode& rhs ) const
    {
        return totalCost() < rhs.totalCost();
    }

    Point prevPos;
    int movementCost;
    int estimatedCost;
};

struct TotalCostComparator
{
    bool operator() ( const PathNode& lhs, const PathNode& rhs ) const
    {
        return lhs.totalCost() < rhs.totalCost();
    }
};

struct PathNodePositionPred
{
    PathNodePositionPred( const PathNode& compareTo )
        : mCompareTo( compareTo )
    {
    }

    bool operator() ( const PathNode& node ) const
    {
        return node.position() == mCompareTo.position();
    }

    const PathNode& mCompareTo;
};

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
        assert( p.x < static_cast<int>(mWidth) &&
                p.y < static_cast<int>(mHeight) );
        mTiles[ p.y * mWidth + p.x ] = c;
    }

    char get( const Point& p ) const
    {
        assert( p.x < static_cast<int>(mWidth) &&
                p.y < static_cast<int>(mHeight) );
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

    size_t width() const
    {
        return mWidth;
    }

    size_t height() const
    {
        return mHeight;
    }

private:
    size_t mWidth;
    size_t mHeight;
    std::vector<char> mTiles;
};

/**
 * Pathfinder is a configurable A* path finding class
 */
class PathFinder
{
public:
    /**
     * Path finder constructor
     */
    PathFinder( const Tilemap& map )
        : mMap( map ),
          mOpenClosedState( map.width() * map.height() ),
          mOpenNodeStore( map.width() * map.height() / 4 )
    {
    }

    /**
     * Using A*, this method attempts to find the shortest path between
     * the original starting location and the requested destination location.
     *
     * This method will make a good faith attempt to discover a path to the
     * destination, but if it cannot it will return false.
     */
    std::vector<Point> findPath( const Point& start, const Point& dest )
    {
        reset();

        std::priority_queue<PathNode> openNodes;
        std::vector<Point> neighbors;

        // Kick the search off by inserting the starting point
        openNodes.push( start );

        std::cout << "Starting findpath... start: "
                  << start << ", end: "
                  << dest
                  << std::endl;

        //
        // Now start the iterative path finding operation by continually
        // trying to find a path to the destination
        //
        bool keepSearching = true;

        for ( size_t count = 0; count < 25 && keepSearching; ++count )
        {
            // Perform the next a* step
            keepSearching = findPathStep( dest, openNodes )
        }

        //
        // Generate the back path
        //
        std::vector<Point> path;

        return path;
    }

private:
    bool findPathStep( const Point& dest,
                       std::priority_queue<PathNode>& openNodes )
    {
        std::cout << " " << std::endl;
        std::cout << "______________________________________" << std::endl;
        std::vector<Point> neighbors;

        // What is the next available and lowest cost tile to consider?
        Point currentPos = openNodes.top();
        openNodes.pop();

        std::cout << "picked: " << currentPos << std::endl;

        // Make sure the tile is marked as closed to prevent revisits
        markClosed( currentPos );

        // Is this the destination tile? If so, we're done!
        if ( currentPos == dest )
        {
            std::cout << "found dest: " << currentPos << std::endl;
            return false;
        }

        // Who will be our neighbors? Will they be our friends?
        generateNeighbors( currentPos, neighbors );

        // Consider each generated neighbor. If the tile is both a valid
        // walkable tile, is in bounds and has not been visited yet then
        // calculate its cost and add it to the list of open tiles
        for ( size_t i = 0; i < neighbors.size(); ++i )
        {
            // Calculate the costs of moving to this node, and the
            // estimated cost of getting to the destination
            int movementCost = findMovementCost( neighbors[i], currentPos );
            int estimateCost = findEstimatedCost( neighbors[i], dest );

            // Double check that this node is pathable. If the movement
            // score is less than zero, we cannot reach this node
            if ( movementCost < 0 )
            {
                continue;
            }

            // If its in the list of closed nodes, don't bother adding it
            if ( isClosed( neighbors[i] ) )
            {
                continue;
            }

            // Generate a node structure
            PathNode nNode( neighbors[i],
                            currentNode.position(),
                            movementCost,
                            estimateCost );

            // Now is this node already listed on the open list? Search the
            // list of open nodes, and see if we find a match
            PathNodePositionPred pred( nNode );

            auto existingNode = std::find_if( openNodes.begin(),
                                              openNodes.end(),
                                              pred
            );

            if ( isOpen( neighbors[i] ) ) 
            {
                // The node is already in the list of open tiles. Do we have
                // a better movement score than the already stored node?
                if ( movementCost < existingNode->movementCost() )
                {
                    *existingNode = nNode;
                }
            }
            else
            {
                // Its not listed in the open list. Add it!
                openNodes.push( nNode );
            }
        }

        // Done with the iteration of a*. Looks like we haven't found
        // the destination node yet
        return true;
    }

    /**
     * Calculates the estimated cost for moving from the given current tile
     * location to the destination tile location. This is a conservative
     * cost estimator that attempts to never over estimate the actual cost,
     * and is needed for accurate a* path finding
     *
     * \param  baseCost  The cost accumulated for getting to the current tile
     * \param  current   The current tile we are estimating cost for
     * \param  dest      Destination tile we are trying to reach
     * \return The estimated cost of moving from current to destination
     */
    int findEstimatedCost( const Point& current, const Point& dest ) const
    {
        return std::abs( current.x - dest.x ) +
               std::abs( current.y - dest.y );
    }

    /**
     * Calculates the (exact) cost of moving from the starting path node
     * to this node, using the path that the current node followed from the
     * start
     */
    int findMovementCost( const Point& currentPt, const Point& prevPoint ) const
    {
        const int INVALID_MOVE  = -1;
        const int MOVE_STRAIGHT = 10;
        const int MOVE_DIAGONAL = 14;
    
        int movementCost = 0;

        // Is this node pathable?
        if (! isPathable( currentPt ) )
        {
            return INVALID_MOVE;
        }

        // IS the path from this node to its parent a straight one, or
        // a diagonal one?
        assert( currentPt != prevPt );

        if ( currentPt.x == prevPt.x || currentPt.y == prevPt.y )
        {
            movementCost += MOVE_STRAIGHT;
        }
        else
        {
            movementCost += MOVE_DIAGONAL;
        }
    
        // Factor in any additional movement costs
        movementCost += 0;

        // All done
        return movementCost;
    }

    /**
     * Given a list of nodes to consider, this method will return the node
     * with the lowest non-negative score in the list. If there is a tie
     * between two or more elements, the node with the greatest index will
     * be used.
     *
     * \param  elements  An array of open tiles to consider
     * \return The tile location with the lowest estimated cost
     */
    PathNode pickNextCheapestNode( std::vector<PathNode>& nodes ) const
    {
        assert( nodes.size() > 0 );  // otherwise this makes no sense
        TotalCostComparator comparator;

        // Find the lowest cost element in the list
        auto itr = std::min_element( nodes.begin(),
                                     nodes.end(),
                                     comparator );
        PathNode node = *itr;

        // Now remove that element from the list before returning it
        nodes.erase( itr );
        return node;
    }

    /**
     * Generates a list of all potentially valid points located adjacent
     * to the requested point 'pt', and stores them into the candidates
     * array. Note that the list of neighbors may not out of bounds, or
     * unwalkable so it is up to the caller to test and discard these incorrect
     * locations
     *
     * \param  pt          The point to generate neighbors for
     * \param  candidates  An array to place the neighbor positions into
     */
    void generateNeighbors( const PathNode& baseNode,
                            std::vector<Point>& output ) const
    {
        const size_t MAX_NEIGHBOR_TILES = 4;
        const int NEIGHBOR_OFFSETS[MAX_NEIGHBOR_TILES][2] =
        {
            {  0, -1 },     // north
            {  1,  0 },     // east
            {  0,  1 },     // south
            { -1,  0 }      // west
        };

        // Consider all the potential neighbor offsets, and generate
        // new pathnode candidates for any that are in bounds and pathable
        int mapWidth  = static_cast<int>( mMap.width() );
        int mapHeight = static_cast<int>( mMap.height() );

        for ( size_t i = 0; i < MAX_NEIGHBOR_TILES; ++i )
        {
            Point basePt = baseNode.position();
            Point nPoint( basePt.x + NEIGHBOR_OFFSETS[i][0],
                          basePt.y + NEIGHBOR_OFFSETS[i][1] );

            if ( nPoint.x >= 0 && nPoint.x < mapWidth  &&
                 nPoint.y >= 0 && nPoint.y < mapHeight && isPathable( nPoint ))
            {
                output.push_back( nPoint );
            }
        }
    }

    void reset()
    {
        // Reset the open/closed state tracking
        std::fill( mOpenClosedState.begin(), mOpenClosedState.end(), 0 );
    }

    /**
     * Checks if the given position allows someone to path to it
     */
    bool isPathable( const Point& ) const
    {
        return true;
    }

private:
    const Tilemap& mMap;
    std::vector<PathNode> mNodeMap;
    std::vector<Point> mOpenStore;
};

void carvePath( Tilemap& map, const Point& start, const Point& end )
{
    PathFinder pathfinder( map );

    // Find the path
    std::vector<Point> path = pathfinder.findPath( start, end );
    assert( path.size() > 0 );

    // Now carve that path
    for ( size_t i = 0; i < path.size(); ++i )
    {
        map.set( path[i], '*' );
    }
}

int main( int, char*[] )
{
    Tilemap map( 78, 20 );
    carvePath( map, Point( 5, 10 ), Point( 8, 11 ) );

    map.print();

}

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>

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

class PathNode
{
public:
    /**
     * Origin path node constructor.
     */
    PathNode( const Point& origin )
        : mPos( origin ),
          mPrevPos( origin ),
          mMovementCost( 0 ),
          mEstimatedCost( 0 )
    {
    }

    /**
     * Non-origin path node constructor
     */
    PathNode( const Point& pt,
              const Point& prevPt,
              int movementCost, 
              int estimatedCost )
        : mPos( pt ),
          mPrevPos( prevPt ),
          mMovementCost( movementCost ),
          mEstimatedCost( estimatedCost )
    {
        assert( movementCost > 0 );
        assert( estimatedCost > 0 );
        assert( prevPt.x >= 0 && prevPt.y >= 0 );
    }

    Point position() const
    {
        return mPos;
    }

    int totalCost() const
    {
        return mMovementCost + mEstimatedCost;
    }

    int movementCost() const
    {
        return mMovementCost;
    }

    int estimatedCost() const
    {
        return mEstimatedCost;
    }

private:
    Point mPos;
    Point mPrevPos;
    int mMovementCost;
    int mEstimatedCost;
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
        : mMap( map )
    {
    }

    /**
     * Using A*, this method attempts to find the shortest path between
     * the original starting location and the requested destination location.
     *
     * This method will make a good faith attempt to discover a path to the
     * destination, but if it cannot it will return false.
     */
    std::vector<Point> findPath( const Point& start, const Point& dest ) const
    {
        std::vector<PathNode> openNodes;
        std::vector<PathNode> closedNodes;
        std::vector<Point> neighbors;

        // Kick the search off by inserting the starting point
        openNodes.push_back( PathNode( start ) );

        //
        // Now start the iterative path finding operation by continually
        // trying to find a path to the destination
        //
        bool keepSearching = true;

        while ( keepSearching )
        {
            // Perform the next a* step
            keepSearching = findPathStep( dest,
                                          openNodes,
                                          closedNodes );
        }

        //
        // Generate the back path
        //
        std::vector<Point> path;

        return path;
    }

private:
    bool findPathStep( const Point& dest,
                       std::vector<PathNode>& openNodes,
                       std::vector<PathNode>& closedNodes ) const
    {
        std::vector<Point> neighbors;

        // What is the next available and lowest cost tile to consider?
        PathNode currentNode = pickNextCheapestNode( openNodes );

        // Make sure the tile is marked as closed to prevent revisits
        closedNodes.push_back( currentNode );

        // Is this the destination tile? If so, we're done!
        if ( currentNode.position() == dest )
        {
            return false;
        }

        // Who will be our neighbors? Will they be our friends?
        generateNeighbors( currentNode.position(), neighbors );

        // Consider each generated neighbor. If the tile is both a valid
        // walkable tile, is in bounds and has not been visited yet then
        // calculate its cost and add it to the list of open tiles
        for ( size_t i = 0; i < neighbors.size(); ++i )
        {
            // Calculate the costs of moving to this node, and the
            // estimated cost of getting to the destination
            int movementCost = findMovementCost( neighbors[i],  currentNode );
            int estimateCost = findEstimatedCost( neighbors[i], dest );

            // Double check that this node is pathable. If the movement
            // score is less than zero, we cannot reach this node
            if ( movementCost < 0 )
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

            if ( existingNode != openNodes.end() ) 
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
                openNodes.push_back( nNode );
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
    int findMovementCost( const Point& currentPt,
                          const PathNode& parent ) const
    {
        const int INVALID_MOVE  = -1;
        const int MOVE_STRAIGHT = 10;
        const int MOVE_DIAGONAL = 14;
    
        int movementCost = 0;
        Point prevPt     = parent.position();

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

        return *std::min_element( nodes.begin(), nodes.end(), comparator );
    }

    /**
     * Checks if the given position is listed as a closed node
     */
    bool isClosed( const Point& pos, const std::vector<PathNode>& nodes ) const
    {
        PathNodePositionPred pred( nNode );

        auto existingNode = std::find_if( openNodes.begin(),
                                            openNodes.end(),
                                            pred
        );
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
        for ( size_t i = 0; i < MAX_NEIGHBOR_TILES; ++i )
        {
            Point basePt = baseNode.position();
            Point nPoint( basePt.x + NEIGHBOR_OFFSETS[i][0],
                          basePt.y + NEIGHBOR_OFFSETS[i][1] );

            if ( nPoint.x >= 0 && nPoint.x < mMap.width()  &&
                 nPoint.y >= 0 && nPoint.y < mMap.height() &&
                 isPathable( nPoint ) )
            {
                output.push_back( nPoint );
            }
        }
    }

    /**
     * Checks if the given position allows someone to path to it
     */
    bool isPathable( const Point& pt ) const
    {
        return true;
    }

private:
    const Tilemap& mMap;
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

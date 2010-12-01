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
#include "octree.cpp"

/////////////////////////////////////////////////////////////////////////////
// HEADER
/////////////////////////////////////////////////////////////////////////////
#ifndef SCOTT_WORKBENCH_GAMEWORLD_H
#define SCOTT_WORKBENCH_GAMEWORLD_H
#define GAMEWORLD_VERSION 2

#include <stdint.h>

const int MATERIAL_NULL = 0;
class CubeWorld;
struct CubeData;

/**
 * This is the class that determines how materials interact with each other,
 * and implements custom behaviors when specific actions occur in the game world.
 * Any cube (or potentially any object) that has registered material type will
 * be called
 */
class AbstractMaterialData
{
public:
    bool onCreateCube( CubeWorld& world, CubeData& data, const Point& pos );
    bool onDestroyCube( CubeWorld& world, CubeData& data, const Point& pos );
    bool onCubeTick( CubeWorld& world, CubeData& data, const Point& pos );

private:
};

typedef uint8_t BaseMaterialId;
typedef uint8_t SubMaterialId;

// Material flags?
//  - void
//  - unpassable
//  - unbreakable
//  - water
//  - uncreatable

// Temporary until we get custom registration working
enum BuiltInMaterials
{
    MAT_EMPTY         =  0,     /* empty void of emptiness */
    MAT_WALL          =  1,     /* unbreakable wall of voidness */
    MAT_AIR           =  2,     /* essentially invisible cube of air */
    MAT_WATER         =  5,     /* water! */
    MAT_GRASS         =  6,
    MAT_DIRT          =  7,
    MAT_ROCK          =  8,
    MAT_WOOD          =  9,
    MAT_VEGETATION    = 10,
};

struct MaterialId
{
    MaterialId( BuiltInMaterials mat )
        : base( mat ),
          sub( 0 )
    {
    }

    MaterialId( BuiltInMaterials mat, uint8_t subid )
        : base( mat ),
          sub( subid )
    {
    }

    BaseMaterialId base;
    SubMaterialId  sub;
};

struct CubeData
{
    CubeData();
    CubeData( const MaterialId& mat )
        : m_material( mat )
    {
    }

    BaseMaterialId baseMaterial() const { return 12; }
    SubMaterialId  subMaterial()  const { return 22; }

    //---------------------------------------------------------------------
    // Helper methods
    //---------------------------------------------------------------------
    static CubeData CreateNullCube();

    //---------------------------------------------------------------------
    // Base data
    //---------------------------------------------------------------------
    MaterialId m_material;
};

typedef TCubeGrid<CubeData>     OctreeChunk;
typedef TCubeGridNode<CubeData> OctreeCubeNode;

class CubeWorld
{
public:
    /**
     * Create a new world of cubes in a volume specified by rows x cols x
     * depth.
     *
     * To store these cubes, the cube world will instantiate cubechunks of
     * dimension dim.
     */
    CubeWorld( size_t chunkDim, size_t rows, size_t cols, size_t depth );

    /**
     * Cube world destructor
     */
    ~CubeWorld();

    /**
     * Places a cube in the world
     */
    void set( const CubeData& value, const Point& pt );

    /**
     * Returns a cube from the world. It will return null cube if you request
     * a location that does not exist.
     */
    CubeData get( const Point& pt ) const;

    /**
     * Returns a cube from the world. This method (non-const) will return a new
     * cube (type empty) if there was no cube in the specified position
     */
    CubeData get( const Point& pt );

    /**
     * Tests if there is a cube at the specified location in the game world
     */
    bool exists( const Point& pt ) const;

    /**
     * Returns the number of chunks instantiated by this cube world
     * FIXME: Make it work
     */
    size_t chunkCount() const;

    /**
     * Returns the number of chunks that comprise this cube world. Note that
     * the number returned is not the number of chunks instantiated, rather
     * the total number of chunks that could possibly be instantiated in the
     * world
     */
    size_t maxChunkCount() const;

    /**
     * Return the number of chunks along the world's column axis
     */
    size_t chunkCols() const { return m_chunkCols; }

    size_t chunkRows() const { return m_chunkRows; }

    size_t chunkDepth() const { return m_chunkDepth; }

    size_t rows() const { return m_rows; }

    size_t cols() const { return m_cols; }

    size_t depth() const { return m_depth; }

private:
    OctreeChunk* findChunk( const Point& p ) const;

    OctreeChunk* findChunk( const Point& p, bool createIfNotFound );

    size_t chunkOffset( const Point& p ) const;

    /**
     * Converts a point into a cube's local space
     */
    Point calcRelativeChunkPoint( const Point& p ) const;

    /**
     * Instantiates a cube chunk
     */
    OctreeChunk* createChunk( const Point& p );

    void checkPointBounds( const Point& p ) const;

    std::vector<OctreeChunk*> m_chunks;

    size_t m_chunkDim;

    /**
     * Number of columns in the world grid
     */
    size_t m_cols;

    /**
     * Number of rows in the world grid
     */
    size_t m_rows;

    /**
     * Depth of columns*rows in the world grid
     */
    size_t m_depth;

    /**
     * Number of columns in the chunks array
     */
    size_t m_chunkCols;

    /**
     * Number of rows in the chunks array
     */
    size_t m_chunkRows;

    /**
     * Number of cols*rows in the chunks array
     */
    size_t m_chunkDepth;

    /**
     * Number of total possible chunks
     */
    size_t m_totalChunks;

    /**
     * Number of chunks instantiated
     */
    size_t m_createdChunkCount;
};


#endif

/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
#include <cassert>
#include <cmath>
#include <vector>


CubeData::CubeData()
    : material( MATERIAL_NULL )
{
}

CubeData::CubeData( int i )
    : material( i )
{
}


CubeData CubeData::CreateNullCube()
{
    return CubeData();
}

CubeWorld::CubeWorld( size_t chunkDims, size_t rows, size_t cols, size_t depth )
    : m_chunkDim( chunkDims ),
      m_cols( cols ),
      m_rows( rows ),
      m_depth( depth ),
      m_chunkCols( cols / chunkDims ),
      m_chunkRows( rows / chunkDims ),
      m_chunkDepth( depth / chunkDims ),
      m_totalChunks( cols/chunkDims + rows/chunkDims + depth/chunkDims ),
      m_createdChunkCount( 0 )
{
    //
    // Ensure that the given cols, rows and depth values are evenly divisble
    // by the chunk dimension
    //
    assert( rows  % chunkDims == 0 );
    assert( cols  % chunkDims == 0 );
    assert( depth % chunkDims == 0 );

    //
    // Calculate number of chunks that need to be created, and then initialize
    // the chunk cache to be all NULL
    //
    float cd      = static_cast<float>(chunkDims);
    m_totalChunks = ceil( static_cast<float>(cols)  / cd ) *
                    ceil( static_cast<float>(rows)  / cd ) *
                    ceil( static_cast<float>(depth) / cd );

    std::cout << m_totalChunks << std::endl;

    //
    // Set up chunk cache
    //
    m_chunks.reserve( m_totalChunks );

    for ( size_t i = 0; i < m_totalChunks; ++i )
    {
        m_chunks[i] = NULL;
    }
}

CubeWorld::~CubeWorld()
{
    for ( size_t i = 0; i < m_chunks.size(); ++i )
    {
        delete m_chunks[i];
    }
}

void CubeWorld::set( const CubeData& value, const Point& pt )
{
    // sanity
    checkPointBounds( pt );

    // Find the chunk that holds the cube
    OctreeChunk * chunk = m_chunks[ chunkOffset(pt) ];

    if ( chunk == NULL )
    {
        chunk = createChunk( pt );
    }

    // Assign the value
    assert( chunk != NULL );
    chunk->set( value, calcRelativeChunkPoint(pt) );
}

CubeData CubeWorld::get( const Point& pt ) const
{
    checkPointBounds( pt );

    // Find the chunk that holds the cube, and query the chunk for
    // the associated cube data. If no such cube chunk exists, then
    // simply return a null cube
    OctreeChunk * chunk = findChunk( pt );

    if ( chunk == NULL )
    {
        return CubeData::CreateNullCube();
    }
    else
    {
        return chunk->get( calcRelativeChunkPoint( pt ) );
    }
}

CubeData CubeWorld::get( const Point& pt )
{
    // Sanity
    checkPointBounds( pt );

    // Find the chunk that holds the cube, and query the
    // chunk for the associated cube data
    OctreeChunk * chunk = findChunk( pt, true );

    return chunk->get( calcRelativeChunkPoint(pt) );
}

bool CubeWorld::exists( const Point& pt ) const
{
    // sanity
    checkPointBounds( pt );

    // Does there exist a pointer to the requested cube?
    OctreeChunk * chunk = findChunk( pt );

    if ( chunk )
    {
        return chunk->exists( calcRelativeChunkPoint(pt) );
    }
    else
    {
        return false;
    }
}

size_t CubeWorld::chunkCount() const
{
    return m_createdChunkCount;
}

size_t CubeWorld::maxChunkCount() const
{
    return m_chunkCols * m_chunkRows * m_chunkDepth;
}

OctreeChunk* CubeWorld::findChunk( const Point& p ) const
{
    return m_chunks[ chunkOffset(p) ];
}

OctreeChunk* CubeWorld::findChunk( const Point& p, bool createIfNotFound )
{
    OctreeChunk* chunk = m_chunks[ chunkOffset(p) ];

    if ( createIfNotFound && chunk == NULL )
    {
        chunk = createChunk( p );
    }

    return chunk;
}

size_t CubeWorld::chunkOffset( const Point& p ) const
{
    // not sure if correct
    const int& OCD = m_chunkDim;

    size_t r = (p.z / OCD) * m_chunkCols * m_chunkRows +
               (p.y / OCD) * m_chunkCols               +
               (p.x / OCD);

    assert( r < m_totalChunks );

    return r;
}

Point CubeWorld::calcRelativeChunkPoint( const Point& p ) const
{
    return Point( p.x % m_chunkDim,
                  p.y % m_chunkDim,
                  p.z % m_chunkDim );
}

/**
  * Instantiates a cube chunk
  */
OctreeChunk* CubeWorld::createChunk( const Point& p )
{
    int offset = chunkOffset( p );

    // Make sure it hasn't been created already
    assert( m_chunks[offset] == NULL );

    // Create it
    OctreeChunk* chunk  = new OctreeChunk( m_chunkDim );

    m_chunks[offset] = chunk;

    return chunk;
}

void CubeWorld::checkPointBounds( const Point& p ) const
{
    assert( p.x >= 0 && static_cast<size_t>(p.x) < m_cols );
    assert( p.y >= 0 && static_cast<size_t>(p.y) < m_rows );
    assert( p.z >= 0 && static_cast<size_t>(p.z) < m_depth );
}

/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cassert>
#include <cmath>

#include <googletest/googletest.h>

// todo
//   - implement a callback or some event when the node doesn't
//     exist (so the engine can generate new cubes)
//
//   - use smart pointer to hide cube data from engine. Octree returns
//     smart pointer that points directly to data stored in the cube.
//     Actually, how about forcing the engine to re-store a cube? That way
//     he can edit it on his own time, and maybe allow better threading
//
//   - LRU cache so we can quickly find the last N cubes without having
//     to look up cube constantly

/////////////////////////////////////////////////////////////////////////////
// Material tests
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Cube Data tests
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Game world
/////////////////////////////////////////////////////////////////////////////
TEST(GameWorld,GameWorldChunkDimsSetCorrectly)
{
    //
    // Create a game world 16x8x32 in size, using 4x4x4 chunks
    //
    const size_t dim = 4, r = 16, c = 8, d = 32;
    CubeWorld w( dim, r, c, d );

    EXPECT_EQ( static_cast<size_t>(64), w.maxChunkCount() );
    EXPECT_EQ( static_cast<size_t>( 0), w.chunkCount() );
    EXPECT_EQ( static_cast<size_t>( 4), w.chunkRows() );
    EXPECT_EQ( static_cast<size_t>( 2), w.chunkCols() );
    EXPECT_EQ( static_cast<size_t>( 8), w.chunkDepth() );
}

TEST(GameWorld,GameWorldCubeDimsSetCorrectly)
{
    //
    // Create a game world 16x8x32 in size, using 4x4x4 chunks
    //
    const size_t dim = 4, r = 16, c = 8, d = 32;
    CubeWorld w( dim, r, c, d );

    EXPECT_EQ( static_cast<size_t>(64), w.maxChunkCount() );
    EXPECT_EQ( static_cast<size_t>(16), w.rows()   );
    EXPECT_EQ( static_cast<size_t>( 8), w.cols()   );
    EXPECT_EQ( static_cast<size_t>(32), w.depth()  );
}

TEST(GameWorld,GameWorldSetSingleCube)
{
    const size_t dim = 4, r = 16, c = 8, d = 32;
    CubeWorld w( dim, r, c, d );

    w.set( CubeData(42), Point( 2, 3, 5 ) );

    CubeData data = w.get( Point( 2, 3, 5 ) );
 //   EXPECT_EQ( 42, data.
}

TEST(GameWorld,SimpleTravelAllCubesAndVerify)
{
    //
    // Create a game world 16x8x32 in size, using 4x4x4 chunks
    //
    const size_t dim = 4, r = 16, c = 8, d = 32;
    CubeWorld w( dim, r, c, d );

    int v = 1;

    // Set all values
    for ( size_t z = 0; z < d; ++z )
    {
        for ( size_t y = 0; y < r; ++y )
        {
            for ( size_t x = 0; x < c; ++x )
            {
                w.set( CubeData( v++ ), Point( x, y, z ) );
            }
        }
    }

    v--;        // remove last value

    // Test all values from top
    for ( int z = d-1; z >= 0; --z )
    {
        for ( int y = r-1; y >= 0; --y )
        {
            for ( int x = c-1; x >= 0; --x )
            {
                CubeData cd = w.get( Point(x, y, z) );

                if ( cd.material != v )
                {
                    std::cerr << "failure at " << x << ", " << y
                              << ", " << z << ". Expected " << v
                              << " but got "
                              << w.get( Point(x,y,z) ).material << std::endl;
                }

                EXPECT_EQ( v, cd.material );

                v--;
            }
        }
    }

}

#include <iostream>
#include <cassert>
#include <cstddef>
#include <vector>

/**
 * 3d bitfield volume. Allows you to represent a 3d volume (one bit data)
 */
class Volume
{
public:
    Volume( size_t dim )
        : m_dim( dim ),
          m_volume( new uint8_t[ dim * dim * dim / 8 ] )
    {
        assert( dim >= 4 );
//        assert( Math::isPowerOfTwo( dim ) );

        std::fill( m_volume,
                   m_volume + ((dim * dim * dim / 8)),
                   0 );
    }

    ~Volume()
    {
        delete[] m_volume;
    }

    bool isSet( size_t x, size_t y, size_t z ) const
    {
        return m_volume[ offset(x,y,z) ];
    }

    void set( size_t x, size_t y, size_t z, bool flag=true )
    {
        m_volume[ offset(x,y,z) ] = flag;
    }

    void clear( size_t x, size_t y, size_t z )
    {
        m_volume[ offset(x,y,z) ] = 0;
    }

private:
    inline size_t offset( size_t x, size_t y, size_t z ) const
    {
        return x + y * m_dim + z * m_dim * m_dim;
    }

    size_t  m_dim;
    uint8_t * m_volume;
};

template<typename T>
class ChunkManager
{
public:
    ChunkManager( size_t width, size_t height, size_t depth )
        : m_width( width ),
          m_height( height ),
          m_depth( depth ),
          m_chunkArrayLength( width * height * depth ),
          m_chunks( width * height * depth, NULL )
    {
    }

    ~ChunkManager()
    {
        DeletePointerContainer( m_chunks );
    }

    /**
     * Checks if the requested chunk is unallocated and hence null
     */
    void isChunkNull( size_t x, size_t y, size_t z ) const
    {
        return m_chunks[ offset(x,y,z) ] == NULL;
    }

    /**
     * Sets the specified chunk to point to the pointer 'data'. Method
     * requires the chunk to be null before assigning a value.
     */
    void set( T* data, size_t x, size_t y, size_t z )
    {
        assert( m_chunks[ offset(x,y,z) ] != NULL );

        m_chunks[ offset(x,y,z) ] = data;
    }
     
    /**
     * Returns the chunk at the requested offset (x,y,z). If the chunk
     * is unallocated, the return pointer will be NULL.
     */
    T* get( size_t x, size_t y, size_t z )
    {
        return m_chunks[ offset(x,y,z) ];
    }

    /**
     * Unloads and destroys the chunk at the requested offset (x,y,z).
     */
    void destroyChunk( size_t x, size_t y, size_t z )
    {
        size_t index = offset( x, y, z );

        // Clear it, and SET IT TO NULL
        delete m_chunks[index];

        m_chunks[index] = NULL;
    }

    /**
     * Returns the width of the 3 dimensional volume holding chunks
     */
    size_t width() const
    {
        return m_width;
    }

    /**
     * Returns the height of the 3 dimensional volume holding chunks
     */
    size_t height() const
    {
        return m_height;
    }

    /**
     * Returns the depth of the 3 dimensional volume holding chunks
     */
    size_t depth() const
    {
        return m_depth;
    }

protected:

    /**
     * Caclulates a chunk's index in the chunk array given its 
     * (x,y,z) position
     */
    inline size_t offset( size_t x, size_t y, size_t z )
    {
        return x + y * m_width + z * m_width * m_height;
    }

private:
    size_t m_width;
    size_t m_height;
    size_t m_depth;

    /**
     * Cached value that stores size of the chunk array
     */
    size_t m_chunkArrayLength;

    /**
     * 3 dimensional chunk storage volume
     */
    typename std::vector<T*> m_chunks;
};

class CubeData
{
private:
    int m_type;
    
};

class CubeStorageVolume
{
public:
    CubeStorageVolume( size_t chunkDim,
                       size_t worldWidth,
                       size_t worldHeight,
                       size_t worldDepth );

    ~CubeStorageVolume();

    /**
     * Make trigger the geometry engine to rebuild a chunk. This method
     * should set the dirty flag or something
     */
    CubeData getCubeAt( size_t x, size_t y, size_t z );
    bool setCube( const CubeData& cube, size_t x, size_t y, size_t z );

//    CubeChunk getChunk( size_t chunkId );

private:
 //   ChunkManager<CubeChunk> m_chunkstore;
};

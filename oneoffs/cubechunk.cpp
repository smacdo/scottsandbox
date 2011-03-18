#include <common/geom/point.h>
#include <common/geom/vector.h>

typedef size_t Material;

struct Face
{
    Face( size_t a, size_t b, size_t c )
        : a(a), b(b), c(c)
    {
    }

    size_t operator []( size_t index ) const
    {
        assert( index < 3 );

        switch( index )
        {
            case 0:
                return a;
            case 1:
                return b;
            case 2:
                return c;
        }
    }

    size_t a;
    size_t b;
    size_t c;
};

struct FaceList
{
    void add( const Face& f )
    {
        faces.push_back( f );
    }

    std::vector<Face> faces;
};

struct Cube
{
    Point    pos;
    Material mat;
};

struct CubeVertex
{
    Vec3 pos;
    Vec2 texuv;
};

//
// Cube positions
// TODO: rearrange these to fit correct builder pattern
// (ABCD, not ADCB)
// [AB
// DC] --> ADCB
//
// xyz
// 1. right
// 2. left
// 3. back
// 4. front
// 5. top
// 6. bottom
Vec3 CubePos[6][4] =
{
    { /* right */
        Vec3( 1.0f, 1.0f, 0.0f ),
        Vec3( 1.0f, 0.0f, 0.0f ),
        Vec3( 1.0f, 0.0f,-1.0f ),
        Vec3( 1.0f, 1.0f,-1.0f )
    },
    { /* left */
        Vec3( 0.0f, 1.0f,-1.0f ),
        Vec3( 0.0f, 0.0f,-1.0f ),
        Vec3( 0.0f, 0.0f, 0.0f ),
        Vec3( 0.0f, 1.0f, 0.0f )
    },
    { /* back */
        Vec3( 1.0f, 1.0f,-1.0f ),
        Vec3( 1.0f, 0.0f,-1.0f ),
        Vec3( 0.0f, 0.0f,-1.0f ),
        Vec3( 0.0f, 1.0f,-1.0f )
    },
    { /* front */
        Vec3( 0.0f, 1.0f, 0.0f ),
        Vec3( 0.0f, 0.0f, 0.0f ),
        Vec3( 1.0f, 0.0f, 0.0f ),
        Vec3( 1.0f, 1.0f, 0.0f )
    },
    { /* top */
        Vec3( 1.0f, 1.0f, 0.0f ),
        Vec3( 1.0f, 1.0f,-1.0f ),
        Vec3( 0.0f, 1.0f,-1.0f ),
        Vec3( 0.0f, 1.0f, 0.0f )
    },
    { /* bottom */
        Vec3( 0.0f, 0.0f, 0.0f ),
        Vec3( 0.0f, 0.0f,-1.0f ),
        Vec3( 1.0f, 0.0f,-1.0f ),
        Vec3( 1.0f, 0.0f, 0.0f )
    }
};

//
// Directional mapper
// (in game space)
// 1. +x --> right
// 2. -x --> left
// 3. +y --> back
// 4. -y --> front
// 5. +z --> top
// 6. -z --> bottom
//
Vec3 axes[6] = { Vec3( 1.0f, 0.0f, 0.0f ),
                    Vec3(-1.0f, 0.0f, 0.0f ),
                    Vec3( 0.0f, 1.0f, 0.0f ),
                    Vec3( 0.0f,-1.0f, 0.0f ),
                    Vec3( 0.0f, 0.0f, 1.0f ),
                    Vec3( 0.0f, 0.0f,-1.0f ) };

int main( int argc, char* argv[] )
{
    
}


void build( const std::vector<Cube>& cubes )
{
    std::vector<CubeVertex> vertices;
    std::unordered_set<Point, bool> cubeCache;
    std::unordered_set<CubeVertex, size_t> vertexCache;
    std::unordered_set<Material, FaceList> facegroups;

    for ( size_t index = 0; index < cubes.size(); ++index )
    {
        const Cube& cube = cubes[i];
        Point pos        = cube.pos;

        //
        //// Check each side of the cube to see if it has neighbors
        //
        for ( int k = 0; k < 6; ++k )
        {
            Point np = Point( pos[0] + axes[k][0],
                              pos[1] + axes[k][1],
                              pos[2] + axes[k][2] );
        }
        
        if ( hasCubeAt( 
    }
    }
}

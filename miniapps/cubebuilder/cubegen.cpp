#include <iostream>

class GlMesh
{
private:
    GLVertex * m_vertices;
    
};

class CubeMeshBuilder
{
public:
    /**
     * Submit a face, ordered:
     *
     *     D         C
     *     x---------x
     *     |       / |
     *     |     /   |
     *     |   /     |
     *     | /       |
     *     x---------x
     *     A         B
     */
    void addFace( const Vec3& a, const Vec3& b,
                  const Vec3& c, const Vec3& c )
    {

    }
};

/**
 * Best veretx format for cubes:
 *    float pos[3];      // 12
 *    byte  tex[2];      // 14
 *    byte  color[3];    // 17
 *    short norms[3];    // 23
 *    byte  extra        // 24
 */
struct GLVertex
{
    float pos[3];       // 4x3   -->  12  /  12 bytes
    float tex[2];       // 4x2   -->   8  /  20 bytes
    uint8_t color[3];   // 1x3   -->   3  /  23 bytes  9 bytes left
    uint16_t norms[3];  // 2x3   -->   6  /  29 byes
};

int main( int argc, char* argv[] )
{

}

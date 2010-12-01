/**
 * Reads MD2 files
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <cassert>

#include <GL/glut.h>

#define MD2_MAX_VNORMS 162

const float VNORM_M[ MD2_MAX_VNORMS ][3] = {
#include "md2norms.inc"
};

typedef float vec3_t[3];

struct MD2_Header
{
    uint32_t ident;         // "IDP2" 844121161
    uint32_t version;       // 8
    uint32_t skin_width;
    uint32_t skin_height;
    uint32_t frame_size;
    uint32_t num_skins;
    uint32_t num_verts;
    uint32_t num_st;
    uint32_t num_tris;
    uint32_t num_glcmds;
    uint32_t num_frames;
    uint32_t offset_skins;
    uint32_t offset_st;
    uint32_t offset_tris;
    uint32_t offset_frames;
    uint32_t offset_glcmds;
    uint32_t offset_end;
} __attribute__((__packed__));

struct MD2_TexCoord_t
{
    int16_t s;
    int16_t t;
} __attribute__((__packed__));;

struct MD2_Triangle_t           // a face
{
    uint16_t vertex[3];
    uint16_t tex[3];
} __attribute__((__packed__));

struct MD2_Vertex_t             // single vertex data (pos, normal)
{
    uint8_t v[3];
    uint8_t n;      // normal index
} __attribute__((__packed__));

struct MD2_Frame_t
{
    float scale[3];
    float translate[3];
    char  name[16];
} __attribute__((__packed__));

struct MD2_Skin_t
{
    char name[64];
} __attribute__((__packed__));

struct MD2_Model
{
    MD2_Model( int frameCount, int faceCount,
               MD2_Skin_t * s, MD2_TexCoord_t * t, MD2_Triangle_t * fa,
               MD2_Frame_t * fr, MD2_Vertex_t ** v )
        : frameCount(frameCount), faceCount( faceCount ),
          skins(s), tex(t), faces(fa), frames(fr), verts(v)
    {
    }

    ~MD2_Model()
    {
        delete[] skins; delete[] tex; delete[] faces; delete[] frames;
        
        for ( int i = 0; i < frameCount; ++i )
        {
            delete[] verts[i];
        }

        delete[] verts;
    }

    int              frameCount;
    int              faceCount;
    MD2_Skin_t     * skins;
    MD2_TexCoord_t * tex;
    MD2_Triangle_t * faces;
    MD2_Frame_t    * frames;
    MD2_Vertex_t  ** verts;
};

struct Vertex
{
    Vertex( float px, float py, float pz, float tu, float tv,
            float nx, float ny, float nz )
        : px(px), py(py), pz(pz), tu(tu), tv(tv), nx(nx), ny(ny), nz(nz)
    {
    }

    bool operator == ( const Vertex& v ) const
    {
        return ( px == v.px && py == v.py && pz == v.pz &&
                 tu == v.tu && tv == v.tv &&
                 nx == v.nx && ny == v.ny && nz == v.nz );
    }

    float px, py, pz;
    float tu, tv;
    float nx, ny, nz;
};

Vertex makeVertex( MD2_Frame_t& f, MD2_Vertex_t& v, MD2_TexCoord_t& t )
{
    return Vertex ( f.scale[0] * v.v[0] + f.translate[0],
                    f.scale[1] * v.v[1] + f.translate[1],
                    f.scale[2] * v.v[2] + f.translate[2],
                    t.s, t.t,
                    VNORM_M[v.n][0], VNORM_M[v.n][1], VNORM_M[v.n][2]
   );
}

void calcNormal( float * n, float a[3], float b[3], float c[3] )
{

}

///////////////////////////////////////////////////////////////////////////
// Renders a MD2 model
///////////////////////////////////////////////////////////////////////////
MD2_Model * GModel = NULL;

void renderFrame()
{
    assert( GModel != NULL );
    MD2_Model * m = GModel;

    int frame_id = 0;
    GLfloat vertex[3];

    // load texture

    // Draw the model
    glClear( GL_COLOR_BUFFER_BIT );
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glPushMatrix();
    glRotatef( 160, 1.0, 0.0, 0.0 );


    glBegin( GL_TRIANGLES );

    glColor3f( 0.5, 0.5, 1.0 );
    
    // Draw each face
    for ( int i = 0; i < m->faceCount; ++i )
    {
        MD2_Frame_t    frame = m->frames[frame_id];
        MD2_Triangle_t face  = m->faces[i];

        // Now each vertex
        for ( int j = 0; j < 3; ++ j )
        {
            int offset      = m->faces[i].vertex[j];
            MD2_Vertex_t vt = m->verts[frame_id][offset];

            vertex[0] = frame.scale[0] * vt.v[0] + frame.translate[0];
            vertex[1] = frame.scale[1] * vt.v[1] + frame.translate[1];
            vertex[2] = frame.scale[2] * vt.v[2] + frame.translate[2];

            glVertex3fv( vertex );
//            glNormal3fv( VNORM_M[ vt.n ] );
        }
    }

    glEnd();
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

void glutIdleFunc()
{
    glutPostRedisplay();
}

///////////////////////////////////////////////////////////////////////////
// Loads a MD2 model
///////////////////////////////////////////////////////////////////////////
MD2_Model* loadMd2Model( const char * path )
{
    // 
    // Open the DDS file
    //
    FILE * file = fopen( path, "rb");

    if (! file )
    {
        perror("Failed to load dds file");
        return NULL;
    }

    //
    // Read the DDS header
    //
    MD2_Header header;

    if ( fread( &header, sizeof(MD2_Header), 1, file ) != 1 )
    {
        perror("Failed while loading MD2 header");
        return NULL;
    }

    //
    // Print header information
    //
    printf("%d verts, %d tex, %d tris, %d frames, %d skins of %dx%x\n",
           header.num_verts,
           header.num_st,
           header.num_tris,
           header.num_frames,
           header.num_skins,
           header.skin_width,
           header.skin_height
    );

    //
    // Validate header
    //
    if ( header.ident != 844121161 || header.version != 8 )
    {
        fprintf( stderr, "Header magic or version incorrect. "
                         "Magic: %u, Version: %d\n",
                 header.ident, header.version );
        return NULL;
    }

    //
    // Allocate buffers in preparation for loading
    //
    MD2_Skin_t *     skins  = new MD2_Skin_t[ header.num_skins ];
    MD2_TexCoord_t * tex    = new MD2_TexCoord_t[ header.num_st ];
    MD2_Triangle_t * faces  = new MD2_Triangle_t[ header.num_tris ];
    MD2_Frame_t    * frames = new MD2_Frame_t[ header.num_frames ];
    MD2_Vertex_t  ** verts  = new MD2_Vertex_t*[ header.num_frames ];

    //
    // Read skin data
    //
    fseek( file,  header.offset_skins, SEEK_SET );
    fread( skins, sizeof(MD2_Skin_t), header.num_skins, file );

    for ( int i = 0; i < header.num_skins; ++i )
    {
        printf(" : SKIN: %s\n", skins[i].name);
    }

    //
    // Load texture and triangle buffers
    //
    fseek( file, header.offset_st, SEEK_SET );
    fread( tex,  sizeof(MD2_TexCoord_t), header.num_st, file );

    fseek( file, header.offset_tris, SEEK_SET );
    fread( faces, sizeof(MD2_Triangle_t), header.num_tris, file );

    //
    // Read frame data
    //
    fseek( file, header.offset_frames, SEEK_SET );
    
    for ( int i = 0; i < header.num_frames; ++i )
    {
        verts[i] = new MD2_Vertex_t[ header.num_verts ];
        
        // Read frame "header" (everything but vertex data)
        fread( frames+i, sizeof(MD2_Triangle_t), 1, file );

        // Read frame vertex data
        fread( verts[i], sizeof(MD2_Vertex_t), header.num_verts, file );


        for ( int j = 0; j < header.num_verts; ++j )
        {
            if ( (verts[i][j].n >= 0 && verts[i][j].n < 162) == false )
            {
                printf("verts[%d][%d].n = %d\n", i, j, verts[i][j].n);
 //               assert(false);
            }

            float x = frames[i].scale[0] * verts[i][j].v[0] + frames[i].translate[0];
            float y = frames[i].scale[1] * verts[i][j].v[1] + frames[i].translate[1];
            float z = frames[i].scale[2] * verts[i][j].v[2] + frames[i].translate[2];
        }
    }

//    assert( false );

    //
    // Combine vertex position and texture information into one
    // super vertex.
    //   - Walk the index buffer, and look up the vertex position. Combine
    //     this with each frame's position information, and add the new
    //     super vertex to that frame's vertex buffer.
    //
    for ( int index = 0; index < header.num_tris; ++index )
    {
        // Create the base frame vertex (frame #0)
/*        MD2_Frame_t *    baseFace = frames[0];
        MD2_Triangle_t * baseTri  = tris[ index ];

        Vertex baseV = makeVertex( baseF, verts[0][index], t ); */
    }

    fclose(file);

    return new MD2_Model( header.num_frames, header.num_tris,
                          skins, tex, faces, frames, verts );
}


int main( int argc, char * argv[] )
{
    const char * path = "md2/modelpack6/truck.md2";
    GModel = loadMd2Model( path );

    int w = 640, h = 480;
    float x = 0.0, y = -100.0, z = 300.0f;
    float lx = 0.0, ly = 0.0, lz = -200.0f;
    float ratio;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
    glutInitWindowPosition( 0, 0 );
    glutInitWindowSize( 640, 480 );
    glutCreateWindow( "Render Preview" );

    glutIdleFunc( glutIdleFunc );
    glutDisplayFunc( renderFrame );

    glClearColor(0,0,0,0);

    ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    	glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, 
		  	x + lx,y + ly,z + lz,
			0.0f,1.0f,0.0f);

    // lighting
    float amb[]  = { 0.7, 0.5, 1.0 };
    float mat[]  = { 0.2, 0.0, 0.5 };
    float spec[] = { 1.0f, 1.0f, 1.0f };
    float shiny[]   = {  50.0f };
    float pos[3] = { 0.0, 0.0, 100.0 };

    glMaterialfv( GL_FRONT, GL_AMBIENT, amb );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat );
    glMaterialfv( GL_FRONT, GL_SPECULAR, spec );
    glMaterialfv( GL_FRONT, GL_SHININESS, shiny );
    glMaterialfv( GL_LIGHT0, GL_POSITION, pos );

//    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
//    glEnable( GL_DEPTH_TEST );

    glutMainLoop();

    delete GModel;
}

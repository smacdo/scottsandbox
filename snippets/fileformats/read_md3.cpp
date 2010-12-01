/**
 * MD3 loader
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <GL/gl.h>

#define MAX_QPATH 64
#define MAX_FRAME_NAME 16
typedef vec3_t float[3];

struct MD3_Header
{
    uint32_t           magic;             // 0x51806873
    int32_t            version;
    uint8_t[MAX_QPATH] pathname;
    uint32_t           flags;
    uint32_t           frames;
    uint32_t           tags;
    uint32_t           surfaces;
    uint32_t           skins;
    uint32_t           frames_offset;
    uint32_t           tags_offset;
    uint32_t           surfaces_offset;
    uint32_t           eof_offset;
} __attribute__((__packed__));;

struct MD3_Frame
{
    vec3_t             min_bounds;
    vec3_t             max_bounds;
    vec3_t             local_origin;    // should be (0,0,0)
    float              radius;
    uint8_t[MAX_FRAME_NAME] name;        
} __attribute__((__packed__));

struct MD3_Tag
{
    uin8t_t[MAX_QPATH] name;
    vec3_t             origin;
    vec3_t             x_orient;
    vec3_t             y_orient;
    vec3_t             z_orient;
} __attribute__((__packed__));

struct MD3_Surface
{

} __attribute__((packed__));

//
// DDS Surface types
//
static const RGBFormat RGB_FORMATS[] =
{
    { "R3G3B2"     ,  8,       0xe0,       0x1c,       0x03,       0x00,
        GL_RGB , GL_RGB , GL_UNSIGNED_BYTE_3_3_2 },
    { "R5G6B5"     , 16,     0xf800,     0x07e0,     0x001f,     0x0000,
        GL_RGB , GL_RGB , GL_UNSIGNED_SHORT_5_6_5 },
    { "A1R5G5B5"   , 16,     0x7c00,     0x03e0,     0x001f,     0x8000,
        GL_RGBA, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV },
    { "X1R5G5B5"   , 16,     0x7c00,     0x03e0,     0x001f,     0x0000,
        GL_RGB , GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV },
    { "A4R4G4B4"   , 16,     0x0f00,     0x00f0,     0x000f,     0xf000,
        GL_RGBA, GL_BGRA, GL_UNSIGNED_SHORT_4_4_4_4_REV },
    { "X4R4G4B4"   , 16,     0x0f00,     0x00f0,     0x000f,     0x0000,
        GL_RGB , GL_BGRA, GL_UNSIGNED_SHORT_4_4_4_4_REV },
    { "A8R3G3B2"   , 16,     0x00e0,     0x001c,     0x0003,     0xff00,
        GL_RGBA, GL_BGRA, FORMAT_UNSUPPORTED },
    { "R8G8B8",      24,   0xff0000,   0x00ff00,   0x0000ff,   0x000000,
        GL_RGB , GL_BGR , GL_UNSIGNED_BYTE }, 
    { "B8G8R8",      24,   0x0000ff,   0x00ff00,   0xff0000,   0x000000,
        GL_RGB , GL_RGB , GL_UNSIGNED_BYTE }, 
    { "A8R8G8B8",    32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000,
        GL_RGBA, GL_BGRA, GL_UNSIGNED_BYTE }, 
    { "X8R8G8B8",    32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000,
        GL_RGB , GL_BGRA, GL_UNSIGNED_BYTE }, 
    { "A8B8G8R8",    32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000,
        GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE }, 
    { "X8B8G8R8",    32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000,
        GL_RGB , GL_RGBA, GL_UNSIGNED_BYTE }, 
    { "A2R10G10B10", 32, 0x000003ff, 0x000ffc00, 0x3ff00000, 0xc0000000,
        GL_RGBA, GL_BGRA, GL_UNSIGNED_INT_2_10_10_10_REV }, 
    { "A2B10G10R10", 32, 0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000,
        GL_RGBA, GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV }, 
    { "G16R16",      32, 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000,
        GL_RGB, FORMAT_UNSUPPORTED, GL_UNSIGNED_SHORT }
};

/////////////////////////////////////////////////////////////////////////////
// Takes a DDS tuple of bit masks, and returns the matching RGB texture
// format specification
/////////////////////////////////////////////////////////////////////////////
bool findRGBFormat( const DDS_Header& header, RGBFormat& format )
{
    int elements              = sizeof(RGB_FORMATS) / sizeof(RGBFormat);
    int offset                = -1;
    const DDS_PixelFormat& pf = header.pixelformat; 

    //
    // Iterate through the list of elements, and see if any of them
    // match our header's bit mask
    //
    for ( size_t i = 0; i < elements; ++i )
    {
        const RGBFormat& f = RGB_FORMATS[i];

        if ( pf.rgbBitCount == f.bitcount  &&
             pf.r_bitmask   == f.r_bitmask &&
             pf.g_bitmask   == f.g_bitmask &&
             pf.b_bitmask   == f.b_bitmask &&
             pf.a_bitmask   == f.a_bitmask )
        {
            // Make sure the format is supported
            if ( f.internal_format == FORMAT_UNSUPPORTED ||
                 f.pixel_format    == FORMAT_UNSUPPORTED ||
                 f.data_type       == FORMAT_UNSUPPORTED )
            {
                return false;
            }

            // Return it
            offset = i;
            format = RGB_FORMATS[i];
            break;
        }
    }

    //
    // Make sure we found a format specifier. If we did, simply return it.
    // If not, well then it looks like this DDS file is unsupported :(
    //
    if ( offset > -1 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

/////////////////////////////////////////////////////////////////////////////
// Print readable information about a DDS file
/////////////////////////////////////////////////////////////////////////////
void print_header( const DDS_Header& header )
{
    // Get surface format
    RGBFormat format;
    bool validFormat = findRGBFormat( header, format );

    // Check surface capabilities
    bool mips = ( header.flags & DDSD_MIPMAPCOUNT ) 
                && header.mipmap_count > 0;
    bool depth = ( header.flags & DDSD_DEPTH )
                && header.depth > 0;
    bool alpha = ( header.pixelformat.flags & DDPF_ALPHAPIXELS );
    bool pitch = ( header.flags & DDSD_PITCH );
    bool lsize = ( header.flags & DDSD_LINEARSIZE );

    printf("DDS: height=%u, width=%u, depth=%u, mipcount=%u, fmt=%s\n",
            header.height,
            header.width,
            header.depth,
            header.mipmap_count,
            ( validFormat ? format.name : "-INVALID-" )
          );

    printf("     mips=%s,depth=%s,alpha=%s,pitch=%s,lsize=%s\n",
            ( mips  ? "yes" : "no" ),
            ( alpha ? "yeS" : "no" ),
            ( depth ? "yes" : "no" ),
            ( pitch ? "yes" : "no" ),
            ( lsize ? "yes" : "no" )
          );
}

int main( int argc, char * argv[] )
{
    const char * path = "stones.dds";

    // 
    // Open the DDS file
    //
    FILE * file = fopen(path, "rb");

    if (! file )
    {
        perror("Failed to load dds file");
        return -1;
    }

    //
    // Read the DDS header
    //
    DDS_Header header;

    if ( fread( &header, sizeof(DDS_Header), 1, file ) != 1 )
    {
        perror("Failed while loading DDS header");
        return -2;
    }

    //
    // Validate header
    //
    if ( header.size != 124 )
    {
        fprintf( stderr, "Header size incorrect. Is: %u, Should be: 124\n",
                 header.size );
        return -2;
    }

    if ( ( header.flags & DDSD_CAPS        == 0 ) ||
         ( header.flags & DDSD_HEIGHT      == 0 ) ||
         ( header.flags & DDSD_WIDTH       == 0 ) ||
         ( header.flags & DDSD_PIXELFORMAT == 0 ) )
    {
        fprintf( stderr, "Header is missing required DDS flags. Uh oh\n" );
        return -2;
    }

    print_header( header );

    //
    // Reject exotic DDS formats
    //
    if ( header.pixelformat.flags & DDPF_BUMPDUDV )
    {
        fprintf( stderr, "DDPF_BUMPDUDV not supported\n" );
        return -3;
    }

    if ( header.pixelformat.flags & DDPF_BUMPLUMINANCE )
    {
        fprintf( stderr, "DDPF_BUMPLUMINANCE not supported\n" );
        return -3;
    }

    //
    // Figure out what type of DDS file this is
    //
    if ( header.pixelformat.flags & DDPF_RGB )
    {
        // uncompressed rgb
    }
    else if ( header.pixelformat.flags & DDPF_LUMINANCE )
    {
        // uncompressed luminance
        fprintf( stderr, "Luminance not supported yet\n" );
        return -3;
    }
    else if ( header.pixelformat.flags & DDPF_ALPHA )
    {
        fprintf( stderr, "Alpha not supported yet\n" );
        return -3;
    }
    else if ( header.pixelformat.flags & DDPF_FOURCC )
    {
        fprintf( stderr, "S3TC not supported yet\n" );
        return -3;
    }
    else
    {
        fprintf( stderr, "Unknown DDS file format\n" );
        return -2;
    }

    //
    // Clean up
    //
    fclose(file);
}

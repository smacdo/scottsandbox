/**
 * DirectDraw Surface reader - Reads DirectX .dds files, and converts them
 * into usuable in-memory textures
 *
 * Initially created using Microsoft's "Programming Guide for DDS" as 
 * a file format guide. Heavily inspired (and several DDCAPS structs stolen)
 * from Open Scene Graph's DDS reader
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <cassert>

#include <GL/gl.h>

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
    ((uint32_t)(char)(ch0) | ((uint32_t)(char)(ch1) << 8) |         \
    ((uint32_t)(char)(ch2) << 16) | ((uint32_t)(char)(ch3) << 24))
#endif

#define FORMAT_UNSUPPORTED 0

//
// FourCC codes for DX compressed texture formats
//
#define FOURCC_DXT1 (MAKEFOURCC('D','X','T','1'))
#define FOURCC_DXT2 (MAKEFOURCC('D','X','T','2'))
#define FOURCC_DXT3 (MAKEFOURCC('D','X','T','3'))
#define FOURCC_DXT4 (MAKEFOURCC('D','X','T','4'))
#define FOURCC_DXT5 (MAKEFOURCC('D','X','T','5'))

//
// DDS Surface flags
//
#define DDSD_CAPS          0x00000001
#define DDSD_HEIGHT        0x00000002
#define DDSD_WIDTH         0x00000004
#define DDSD_PITCH         0x00000008
#define DDSD_PIXELFORMAT   0x00001000
#define DDSD_MIPMAPCOUNT   0x00020000
#define DDSD_LINEARSIZE    0x00080000
#define DDSD_DEPTH         0x00800000

//
// DDS pixel format flags
//
#define DDPF_ALPHAPIXELS   0x00000001
#define DDPF_ALPHA         0x00000002
#define DDPF_FOURCC        0x00000004
#define DDPF_RGB           0x00000040
#define DDPF_COMPRESSED    0x00000080
#define DDPF_LUMINANCE     0x00020000
#define DDPF_BUMPLUMINANCE 0x00040000
#define DDPF_BUMPDUDV      0x00080000

//
// DDS capabilities flags
//
#define DDSCAPS_COMPLEX    0x00000008
#define DDSCAPS_TEXTURE    0x00001000
#define DDSCAPS_MIPMAP     0x00400000
#define DDSCAPS2_VOLUME    0x00200000

struct DDS_ColorKey
{
    DDS_ColorKey():
        colorSpaceLowValue(0),
        colorSpaceHighValue(0)
    {
    }
        
    uint32_t colorSpaceLowValue;
    uint32_t colorSpaceHighValue;
} __attribute__((__packed__));;

struct DDS_PixelFormat
{
    DDS_PixelFormat():
        size(0),
        flags(0),
        fourCC(0),
        rgbBitCount(0),
        r_bitmask(0),
        g_bitmask(0),
        b_bitmask(0),
        a_bitmask(0)
    {
    }
        
    uint32_t size;
    uint32_t flags;
    uint32_t fourCC;
    union
    {
        uint32_t rgbBitCount;
        uint32_t yuvBitCount;
        uint32_t zBufferBitDepth;
        uint32_t alphaBitDepth;
        uint32_t luminanceBitDepth;
    };
    union
    {
        uint32_t r_bitmask;
        uint32_t y_bitmask;
    };
    union
    {
        uint32_t g_bitmask;
        uint32_t u_bitmask;
    };
    union
    {
        uint32_t b_bitmask;
        uint32_t v_bitmask;
    };
    union
    {
        uint32_t a_bitmask;
        uint32_t yuvAlphaBitMask;
        uint32_t rgbZBitMask;
        uint32_t yuvZBitMask;
    };
} __attribute__((__packed__));;

struct DDS_Caps
{
    uint32_t caps;
    uint32_t caps2;
    uint32_t caps3;
    union
    {
        uint32_t caps4;
        uint32_t volume_depth;
    };
};

struct DDS_Header
{
    uint32_t size;
    uint32_t flags;
    uint32_t height;
    uint32_t width;
    union
    {
        int32_t   pitch;
        uint32_t  linear_size;
    };
    union
    {
        uint32_t backbuffer_count;
        uint32_t depth;
    };
    union
    {
        uint32_t mipmap_count;
        uint32_t refresh_rate;
    };
    uint32_t        alphabit_depth;
    uint32_t        reserved_1;
    uint32_t        lpsurface_ptr_ignore;
    DDS_ColorKey    dest_overlay;
    DDS_ColorKey    dest_blt;
    DDS_ColorKey    src_overlay;
    DDS_ColorKey    dst_overlay;
    DDS_PixelFormat pixelformat;
    DDS_Caps        caps;
    uint32_t        texture_stage;
} __attribute__((__packed__));

struct RGBFormat
{
    const char * name;          // human readable title
    uint32_t     bitcount;
    uint32_t     r_bitmask;
    uint32_t     g_bitmask;
    uint32_t     b_bitmask;
    uint32_t     a_bitmask;
    unsigned int internal_format;
    unsigned int pixel_format;
    unsigned int data_type;
};

struct TextureData
{
    int width;
    int height;
    unsigned int internal_format;
    unsigned int pixel_format;
    unsigned int data_type;
};

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
    bool mipsc = ( header.pixelformat.flags & DDPF_FOURCC );
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

    printf(" x lsize=%d,pitch=%d\n", (int)header.linear_size,
             header.pitch );

    printf("     mips=%s(%s),depth=%s,alpha=%s,pitch=%s,lsize=%s\n",
            ( mips  ? "yes" : "no" ),
            ( mipsc ? "compressed" : "uncompressed" ),
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
    // Read DDS "magic" value, specifies that this is indeed a DDS file
    //
    static_assert( sizeof(char) == 1, "char should be one byte" );
    char magic[4];

    if ( fread( magic, 1, 4, file ) != 4 )
    {
        perror("Unable to read 4-byte DDS header from DDS texture");
        return -2;
    }

    if ( strncmp( magic, "DDS ", 4 ) != 0 )
    {
        fprintf( stderr, "File missing DDS header. This is not a valid "
                         "DDS texture file!\n");
        fprintf( stderr, "HEADER: %s\n", magic );
        return -2;
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
    // Read texture
    //  (size magic comes from OSG DDS reader)
    //
    size_t texsize;

    if ( header.mipmap_count > 1 )
    {
//        texsize = header.linear_size *
//                   ( header.pixelformat.fourCC == FOURCC_DXT1 ? 2 : 4 );
    }
    else
    {
        texsize = header.linear_size;
    }

    printf("SIZE: texture size is %d\n", (int) texsize);


    //
    // Slurp texture data in
    //
    uint8_t * imagedata = new uint8_t[ texsize ];
    fread( imagedata, 1, texsize, file );


    //
    // Process texture mip-maps (if they exist)
    //
    bool fourCCMipMaps = header.pixelformat.flags & DDPF_FOURCC;
    bool rgbMipMaps    = header.pixelformat.flags & DDPF_RGB;

    assert( fourCCMipMaps == true || rgbMipMaps == true );

    //
    // Are they compressed or uncompressed?
    //
    if ( fourCCMipMaps )        // compressed
    {
    }
    else                        // uncompressed RGB mip maps
    {
        int offset = 0;
        int size   = 0;
        int width  = header.width;
        int height = header.height;

        for ( int j = 0; j < header.mipmap_count && (width||height); ++j )
        {
            if ( width  == 0 ) { width  = 1; }
            if ( height == 0 ) { height = 1; }

            printf("mipmap is %dx%d\n", width,height);

            size    = width * height * 
                        ( header.pixelformat.rgbBitCount/8 );
            offset += size;

            width >>= 1;        // pow2
            height >>= 1;       // pow2
        }

        printf("offset = %d\n", offset);
    }

    //
    // Clean up
    //
    fclose(file);
}

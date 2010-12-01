#include "renderer/model.h"
#include "renderer/texture.h"
#include "core/math.h"
#include "core/logger.h"

#include <cassert>

#include <sstream>
#include <string>
#include <iostream>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MODEL_VTX_ANIM 1
#define MODEL_BONE_ANIM 2

//#define ASSET_LOADER_DEBUG 1
struct AssetHeader
{
    uint8_t transmission_byte;      // Always 89
    char    filetype[5];            // Always "ASSET"
    char    assettype[4];           // "MTRL/MESH"
    uint8_t error_detection[4];     // 13, 10, 26, 10
    uint8_t data_vers;              // Asset data versioning
    uint8_t flags;                  // Currently unused
    char    name[32];               // Name of the asset (null-terminated)
} __attribute__((__packed__));

struct ModelHeader                  // 16 bytes
{
    uint8_t version;                // 1, 1 model type (1=v.anim, 2=bone)
    uint8_t flags;                  // 1, 2 flags for model  
    uint8_t mesh_groups;            // 1, 3 number of mesh groups per frame
    uint8_t anim_groups;            // 1, 4 number of named animations
    int32_t frame_count;            // 4, 8 number of animation frames
    int32_t face_count;             // 4, 12 number of faces (vtx/3)
    int32_t vbuffer_count;          // 4, 16 number of elements in array
} __attribute__((__packed__));

struct GroupInfo
{
    char    name[32];               // 32, 32  name of the mesh subgroup
    char    material[32];           // 32, 64  name of the material for grp
    int32_t face_count;             //  4, 68  number of faces in group
    int32_t face_offset;            //  4, 72  index array offset (*1, not 3)
} __attribute__((__packed__));

struct AnimInfo
{
    char    name[32];               // 32, 32  name of the animation
    int32_t frames;                 //  4, 36  number of frames in animation
    int32_t frames_offset;          //  4, 40  offset in frame vb array
    float   tick_time;              //  4, 44  amount of time per frame
    int32_t reserved;               //  4, 48  nothing
} __attribute__((__packed__));

struct FMaterialHeader              // 4 bytes
{
    uint8_t flags;                  // 1, 1   material flags
    uint8_t passes;                 // 1, 2   number of passes
    uint8_t reserved[2];            // 2, 4   reserved
} __attribute__((__packed__));

struct FMaterialPass                // 64 bytes
{
    float ambient[3];               // 12, 12
    float diffuse[3];               // 12, 24
    float specular[3];              // 12, 36
    float emissive[3];              // 12, 48
    float alpha;                    //  4, 52
    float shininess;                //  4, 56
    uint8_t shading_model;          //  1, 57 (0=none,1=flat,2=goruard,3=ph)
    uint8_t depth_func;             //  1, 58 (0=fail,1=pass,lt,le,eq,ne,gt,ge)
    uint8_t cull_func;              //  1, 59 (0=none,1=cw,2=ccw)
    uint8_t polygon_mode;           //  1, 60 (0=solid,1=wireframe,2=points)
    uint8_t texture_count;          //  1, 61
    uint16_t scene_blend_op;        //  1, 63 (compressed)
    uint8_t flags;                  //  1, 64
} __attribute__((__packed__));

struct FTextureOp                   // 64 bytes
{
    float scale[2];                 //  8,  8  x/y scaling factor
    float rotate[2];                //  8, 16  x/y rotation factor
    float scroll[2];                //  8, 24  x/y scrolling?
    uint8_t scale_type_x;           //  1, 25  texture scaling x axis
    uint8_t scale_type_y;           //  1, 26  texture scaling y axis
    uint8_t color_op;               //  1, 27  texture operation
    uint8_t alpha_color_op;         //  1, 28  alpha texture operation
    uint8_t reserved[4];            //  4, 32  reserved
    char    texturepath[32];        // 32, 64  path to the texture data
} __attribute__((__packed__));

/**
 * Takes a asset header structure, and verifies that the contents of
 * said structure are valid. This includes checking the error bits of the
 * header in addition to ensuring that the asset type name is valid.
 */
bool verifyAssetHeader( const AssetHeader& header, const char* type )
{
    // Verify ASSET header
    if ( header.filetype[0] != 'A' || header.filetype[1] != 'S' ||
         header.filetype[2] != 'S' || header.filetype[3] != 'E' ||
         header.filetype[4] != 'T' )
    {
        std::cerr << "This data does not appear to be an asset"
                  << std::endl;
        return false;
    }

    // Verify MESH type
    if ( header.assettype[0] != type[0] || 
         header.assettype[1] != type[1] ||
         header.assettype[2] != type[2] ||
         header.assettype[3] != type[3] )
    {
        std::cerr << "This asset does not appear to be a asset of type "
                  << type
                  << std::endl;
        return false;
    }

    // Verify error detection bytes are correct
    if ( header.transmission_byte  != 89 ||
         header.error_detection[0] != 13 ||
         header.error_detection[1] != 10 ||
         header.error_detection[2] != 26 ||
         header.error_detection[3] != 10 )
    {
        std::cerr << "Asset error detection bytes are malformed. Did you "
                  << "transfer this file correctly?"
                  << std::endl;
        return false;
    }

    // Looks like it worked
    return true;
}

/**
 * Very similiar to the verifyAssetHeader function. This function
 * checks that the model header's data is both correct and sane. These
 * checks include checking to make sure all values are greater than
 * zero and under a certain maximum limit.
 *
 * Returns true if everything checks out
 */
bool verifyModelHeader( const ModelHeader& header )
{
    // Max limits aren't needed in the future, their current purpose
    // is to easily detect corrupted values. I'll remove them once
    // the model format / toolchain is stable
    if ( header.version != MODEL_VTX_ANIM &&
         header.version != MODEL_BONE_ANIM )
    {
        std::cerr << "Model is of unsupported format '"
                  << header.version << "'" << std::endl;
        return false;
    }

    if ( header.mesh_groups < 1 || header.mesh_groups > 127 )
    {
        std::cerr << "Model has too few / too many groups: "
                  << header.mesh_groups
                  << std::endl;
        return false;
    }

    if ( header.anim_groups < 1 || header.anim_groups > 127 )
    {
        std::cerr << "Model has too few / too many animations: "
                  << header.anim_groups
                  << std::endl;
        return false;
    }

    if ( header.frame_count < 1 || header.frame_count > 65535 )
    {
        std::cerr << "Model has too few / too many animation frames: "
                  << header.frame_count
                  << std::endl;
        return false;
    }

    if ( header.face_count < 1 || header.face_count > 65535 )
    {
        std::cerr << "Model has too few / too many faces: "
                  << header.face_count
                  << std::endl;
        return false;
    }

    if ( header.vbuffer_count < 1 || header.vbuffer_count > 65535 )
    {
        std::cerr << "Model has too few / too many vertices: "
                  << header.vbuffer_count
                  << std::endl;
        return false;
    }

    // Looks good
    return true;
}

/**
 * Given a file path to an model asset, this function will load it, construct
 * an asset to hold this asset and return a pointer to it.
 *
 * Big messy ugly function. We could split it up and make it nicer, but it
 * wouldn't really serve any purpose - a lot of the work is verifying that
 * the values in the file are correct when we load them.
 */
Model loadModel( const char * path )
{
    //
    // Open the file up
    //
    FILE * file  = fopen( path, "rb" );
    int    count = 0;

    if (! file )
    {
        std::cerr << "Failed to load binary mesh: " << path
                  << std::endl;
        assert( false );
    }

    //
    // Read the asset header in
    //
    AssetHeader assetHeader;

    count = fread( &assetHeader, sizeof(AssetHeader), 1, file );

    // Validate the asset header
    if ( count != 1 )
    {
        std::cerr << "Failed to load binary mesh asset header: "
                  << path << std::endl;
        assert( false );
    }

    if (! verifyAssetHeader( assetHeader, "MESH" ) )
    {
        std::cout << "Failed to read asset header" << std::endl;
        assert( false );
    }

    //
    // Read model header
    //
    ModelHeader modelHeader;

    count = fread( &modelHeader, sizeof(ModelHeader), 1, file );

    if ( count != 1 )
    {
        std::cerr << "Model header was corrupted" << std::endl;
        assert( false );
    }

    if (! verifyModelHeader( modelHeader ) )
    {
        std::cerr << "Failed to read model header" << std::endl;
        assert( false );
    }

    // Create local variables to save some typing
    int meshGroupCount      = modelHeader.mesh_groups;
    int animGroupCount      = modelHeader.anim_groups;
    int frameCount          = modelHeader.frame_count;
    int faceCount           = modelHeader.face_count;
    int vBufferElementCount = modelHeader.vbuffer_count;

    //
    // Read mesh sub-group information
    //
    std::vector<Surface*> meshes;
    GroupInfo * groups = new GroupInfo[ meshGroupCount ];
    int totalIndexSize = faceCount * 3;

    count = fread( groups, sizeof(GroupInfo), meshGroupCount, file );

    if ( count != meshGroupCount )
    {
        std::cerr << "Failed to read in correct number of groups. Got "
                  << count << ", expected " << meshGroupCount
                  << std::endl;
        assert( false );
    }

    // Load meshes into mesh array
    for ( int i = 0; i < meshGroupCount; ++i )
    {
        // Load the material
        //   TODO make this look up the material rather than load it
        //        and if it does not exist report a warning?
        //
        //   Shared<Material> mat = Manager<Material>::singleton().get(..)
        std::string path = std::string("content/") + groups[i].material;
        Shared<Material> mat( loadMaterial( path.c_str() ) );

        assert( mat->passes.size() > 0 );
        assert( mat->passes[0].tex_ops.size() > 0 );

        // Convert it into engine format, add it to list
        meshes.push_back( new Surface( mat,
                                       groups[i].face_count,
                                       groups[i].face_offset ) );

        // Debug information
#ifdef ASSET_LOADER_DEBUG
        std::cout << "Mesh Group (#" << i << " ): "
                  << "name='"  << groups[i].name       << "', "
                  << "mat='"   << groups[i].material   << "', "
                  << "faces="  << groups[i].face_count << ", "
                  << "ofs="    << groups[i].face_offset
                  << std::endl;
#endif
    }

    //
    // Read animation groups information
    //
    AnimInfo * anims   = new AnimInfo[ animGroupCount ];

    count = fread( anims, sizeof(AnimInfo), animGroupCount, file );

    // Verify we actually read in correct number of animations
    if ( count != animGroupCount )
    {
        std::cerr << "Failed to read in correct number of animations. Got "
                  << count <<", expected " << animGroupCount
                  << std::endl;
        assert( false );
    }

    // Check that the first animation is the root animation
    assert( animGroupCount >= 1 );

    if ( strncmp( anims[0].name, "__default", strlen("__default") ) != 0 )
    {
        std::cerr << "Expected first animation to be '__default'. Got '"
                  << anims[0].name << "'"
                  << std::endl;
        assert( false );
    }

    //
    // Read index buffers
    //
    uint32_t * index_buffer = new uint32_t[ totalIndexSize ];
    count = fread( index_buffer, sizeof(uint32_t), totalIndexSize, file );

    if ( count != totalIndexSize )
    {
        std::cout << "Failed when reading index buffer. Got "
                  << count << ", expected " << totalIndexSize
                  << std::endl;
        assert( false );
    }

    // Spit out each index
#ifdef ASSET_LOADER_DEBUG
    std::cout << "INDEX BUFFER: ";

    for ( int i = 0; i < totalIndexSize; ++i )
    {
        std::cout << index_buffer[i] << ", ";
    }

    std::cout << std::endl;
#endif

    //
    // Read vertex buffer
    //
    size_t    vertex_size   = sizeof(float) * 8;
    uint8_t * vertex_buffer = new uint8_t[ vertex_size * vBufferElementCount ];

    count = fread( vertex_buffer, vertex_size, vBufferElementCount, file );

    if ( count != vBufferElementCount )
    {
        std::cout << "Failed when reading vertex buffer. Got "
                  << count << ", expected " << vBufferElementCount
                  << std::endl;
        assert( false );
    }

    //
    // Construct a mesh
    //   - NOTE: For the moment we assume each model only has one
    //           group and material, so we basically treat the
    //           index_buffer as one big index_buffer rather than the
    //           (correct) interpretation of it having multiple index_buffers
    //           side by side.
    //
    //           This should be fixed when I'm not being lazy
    //
    std::vector<int> indices(index_buffer, index_buffer + totalIndexSize + 1);

    ModelData * modelData = new ModelData( meshes,
                                           indices,
                                           vertex_buffer,
                                           VertexFormat::Standard,
                                           totalIndexSize / 3,
                                           vBufferElementCount );

    //
    // all done
    //
    fclose( file );

    delete anims;
    delete groups;

    INFO("assetloader") << "Loaded binary mesh: " 
                        << path
                        << " ("
                        << meshGroupCount << " groups, "
                        << animGroupCount << " anims, "
                        << faceCount << " faces, "
                        << vBufferElementCount << " vertices)"
                        << ENDLOG;

    return Model( modelData );
}


Shared<Material> loadMaterial( const char * path )
{
    //
    // Open the file up
    //
    DEBUG("MaterialLoader") << "Starting material load: "
                            << path << ENDLOG;

    FILE * file  = fopen( path, "rb" );
    int    count = 0;

    if (! file )
    {
        raiseError("Failed to open material data file");
    }

    //
    // Read the asset header
    //
    AssetHeader assetHeader;
    count = fread( &assetHeader, sizeof(AssetHeader), 1, file );

    // Validate the asset header
    if ( count != 1 )
    {
        raiseError("Failed to read asset data header");
    }

    if (! verifyAssetHeader( assetHeader, "MTRL" ) )
    {
        raiseError("Failed to verify asset data header");
    }

    //
    // Read material header
    //
    FMaterialHeader materialHeader;
    count = fread( &materialHeader, sizeof(FMaterialHeader), 1, file );

    if ( count != 1 )
    {
        raiseError("Failed to read material data header");
    }

    // TODO verify

    //
    // Load material passes
    //
    std::vector<MaterialPass> stored_passes;

    FMaterialPass * passes = new FMaterialPass[ materialHeader.passes ];
    count = fread( passes, sizeof(FMaterialPass), materialHeader.passes, file );

    if ( count != materialHeader.passes )
    {
        raiseError("Failed to load material passes while loading material");
    }

    for ( int i = 0; i < materialHeader.passes; ++i )
    {
        std::vector<TextureOp> stored_ops;

        FMaterialPass *pass     = &passes[i];

        //
        // TODO validate pass data
        //

        //
        // Load material passes's textures
        //
        FTextureOp * ops = new FTextureOp[ pass->texture_count ];
        count = fread( ops, sizeof(FTextureOp), pass->texture_count, file );

        if ( count != pass->texture_count )
        {
            std::cerr << "Failed to load material texture ops" << std::endl;
            std::cerr << " (pass " << i << ")" << std::endl;
            assert( false );
        }

        for ( int j = 0; j < pass->texture_count; ++j )
        {
            std::string path = std::string("content/") +
                               ops[j].texturepath;
            Shared<Texture> tex( loadTexture( path.c_str()) );
            //std::cout << "TEXTURE " << ops[j].texturepath << std::endl;

            stored_ops.push_back( TextureOp( tex ) );
        }

        assert( stored_ops.size() > 0 );

        // Create and add the material pass
        stored_passes.push_back( MaterialPass( stored_ops,
                                               Color(),
                                               Color(),
                                               Color(),
                                               1.0,
                                               1.0,
                                               1.0 )
        );

        assert( stored_passes[0].tex_ops.size() > 0 );

        // clean up
        delete ops;
    }

    //
    // All done
    //
    fclose( file );

    delete passes;
    
    INFO("assetloader") << "Loaded material: " << path << ENDLOG;
    assert( stored_passes[0].tex_ops.size() > 0 );
    INFO("assetloader") << "done" << ENDLOG;

    return Shared<Material>( new Material( stored_passes ) );
}

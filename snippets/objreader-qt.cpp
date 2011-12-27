#include "stdafx.h"
#include "sandbox.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include <cassert>

struct Vec3
{
    Vec3( float x, float y, float z )
        : x(x), y(y), z(z)
    {
    }
    float x, y, z;
};

struct Vec2
{
    Vec2( float x, float y )
        : x(x), y(y)
    {
    }

    float x,y;
};

struct VertexColor
{
    VertexColor( float r_, float g_, float b_, float a_ )
        : r(r_), g(g_), b(b_), a(a_)
    {
    }

    VertexColor( float r_, float g_, float b_ )
        : r(r_), g(g_), b(b_), a(1.0)
    {
    }

    VertexColor( const Vec3& v )
        : r(v.x), g(v.y), b(v.z), a(1.0)
    {
    }

    float r, g, b, a;
};

struct Face
{
    Face( size_t a_, size_t b, size_t c )
        : a(a_), b(b_), c(c_)
    {
    }

    size_t a;
    size_t b;
    size_t c;
};

struct ObjVertex
{
    Vec3 pos;
    Vec2 tex;
    Vec3 normal;
};

struct ObjGroup
{
    ObjGroup( const std::string& name, size_t offset )
        : name(name), material(), offset(offset), count()
    {
    }

    // Name of the object group
    std::string name;

    // Material used for the group
    std::string material;

    // Where in the face array does this group start
    size_t offset;

    // How many faces does this group contain
    size_t count;
};

struct ObjData
{
    ObjData()
        : positions(),
          texcoords(),
          normals(),
          vertices(),
          vertexCache(),
          faces(),
          hasDeterminedFormat(false),
          hasPosition(false),
          hasTexCoord(false),
          hasNormal(false)
    {
    }

    // List of positions
    std::vector<Vec3> positions;

    // List of texture coordinates
    std::vector<Vec2> texcoords;

    // List of normal vecs
    std::vector<Vec3> normals;

    // List of merged vertices created from the face command
    std::vector<ObjVertex> vertices;

    // Maps a face string to a merged vertex
    std::map<std::string, size_t> vertexCache;

    // List of faces
    std::vector<Face> faces;

    // List of face groups
    std::vector<ObjGroup> groups;

    // The current group being parsed
    std::string currentGroup;

    // Flag if first face has been encountered and specified vertex composition
    bool hasDeterminedFormat;

    // Flag if vertices have position (must be true)
    bool hasPosition;

    // Flag if vertices have tex coordinates
    bool hasTexCoord;

    // Flag if vertices have normal vector
    bool hasNormal;
};

struct Vertex
{
    Vec3 pos;
    Vec2 uv;
    Vec3 normal;
};

struct MeshData
{
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
};

struct LightingData
{
    VertexColor ambient;
    VertexColor diffuse;
    VertexColor specular;
    float specularPower;
};

struct MaterialData
{
    LightingData lighting;
    std::string diffuseMap;
    bool castsShadows;
};

class ResourceLoader
{
public:
    bool loadResourceFile( const QString& resourcePath );
    MeshData* loadMesh( const QString& meshname );
    QImage* loadTexture( const QString& name );

private:
    // List of model paths
    QHash<QString, QString> m_models;
    QHash<QString, QString> m_meshes;
    QHash<QString, QString> m_textures;
    QHash<QString, QString> m_materials;
};

class ResourceManager
{
public:
    QSharedPointer<MeshData> findMesh( const QString& meshName );

private:
    ResourceLoader m_loader;
    QHash<QString, QSharedPointer<MeshData> > m_meshes;
};

QImage ResourceLoader::loadTexture( const QString& name )
{
    QImage texture;

    // Look up the requested texture name, get its resource path
    if (! m_textures.contains( name ) )
    {
        App::RaiseError("ResourceLoader")
            << "Failed to locate resource key for texture: " << name;
    }
    else
    {
        QString filepath = m_texures[ name ];

        // Try to load the texture
        if (! texture.load( filepath ) )
        {
            App::RaiseError("ResourceLoader")
                << "Failed to locate texture file: " << filepath;
        }
    }

    return texture;
}

/////////////////////////////////////////////////////////////////////////////
// Text file parser
/////////////////////////////////////////////////////////////////////////////
class TextFileParser
{
public:
    TextFileParser( const std::string& commentDelims );
    virtual ~TextFileParser();

    bool parseFile( const std::string& filename );
    bool parseString( const std::string& contents );

    virtual bool parse( std::istream& input ) = 0;

    bool hasErrors() const
    {
        return m_hasErrors;
    }

    std::string errorText() const
    {
        return m_errorText;
    }

    std::string lineText() const
    {
        return m_lineText;
    }

    size_t lineNumber() const
    {
        return m_lineNumber;
    }

    std::string filename() const
    {
        return m_filename;
    }

protected:
    virtual void reset();

protected:
    void raiseError( const std::string& message );
    bool isCurrentLineEmpty() const;
    bool readNextLine( std::istream& input );
    
    int parseInt( const std::string& input );
    float parseFloat( const std::string& input );

    void tokenizeCommandString( const std::string& inputLine,
                                std::vector<std::string>& output );
    void tokenizeCommandString( const std::string& inputLine,
                                const std::string& delims,
                                std::vector<std::string>& output );

private:
    std::string m_inputdata;
    std::string m_lineText;
    std::string m_filename;
    size_t m_lineNumber;
    bool m_hasErrors;
    std::string m_errorText;
    const std::string m_commentDelims;
};

TextFileParser::TextFileParser( const std::string& commentDelims )
    : m_inputdata(),
      m_lineText(),
      m_filename("[none]"),
      m_lineNumber( 0 ),
      m_hasErrors( false ),
      m_errorText(),
      m_commentDelims( commentDelims )
{
}

TextFileParser::~TextFileParser()
{
}

bool TextFileParser::parseFile( const std::string& filename )
{
    std::ifstream infile( filename.c_str(), std::ios::in );
    m_filename = filename;

    if (! infile )
    {
        raiseError("Failed to open file");
        return false;
    }
    else
    {
        return parse( infile );
    }
}

bool TextFileParser::parseString( const std::string& contents )
{
    std::istringstream infile( contents );
    return parse( infile );
}

bool TextFileParser::readNextLine( std::istream& input )
{   
    // Read the next line, make sure its not the end of the line. Also
    // make sure to update our current line text / number values
    std::getline( input, m_lineText, '\n' );
    m_lineNumber++;

    // If the read was succesfull, strip all comments and then remove
    // any leading/trailing whitespace
    if ( input.good() )
    {
        // Remove comments
        m_lineText =
            m_lineText.substr(0, m_lineText.find_first_of( m_commentDelims ));

        // Remove leading whitespace
        size_t start = m_lineText.find_first_not_of( " \t\r\n" );
        
        if ( start != std::string::npos )
        {
            m_lineText = m_lineText.erase( 0, start );
        }
        else
        {
            m_lineText = "";
        }

        // Also remove trailing whitespace
        size_t end = m_lineText.find_last_not_of( " \t\r\n" );

        if ( end != std::string::npos && end != m_lineText.size()-1 )
        {
            m_lineText = m_lineText.erase( end, m_lineText.size() - end );
        }
    }

    // All done, let caller know if the read was succesfull or if we're
    // at the end of the line
    return input.good();
}

bool TextFileParser::isCurrentLineEmpty() const
{
    return m_lineText.empty();
}

void TextFileParser::reset()
{
    m_inputdata.clear();
    m_errorText.clear();
    m_lineText.clear();

    m_filename   = "[none]";
    m_lineNumber = 0;
    m_hasErrors  = false;
}

float TextFileParser::parseFloat( const std::string& input )
{
    std::stringstream ss( input );

    // Attempt to convert to float
    float result = 0.0f;
    ss >> result;

    // did it work?
    if ( ss.fail() )
    {
        raiseError("Failed to parse argument as float");
        result = 0.0f;
    }

    // give it back regardless of result
    return result;
}

int TextFileParser::parseInt( const std::string& input )
{
    std::stringstream ss( input );

    // Attempt to convert to an integer
    int result = 0;
    ss >> result;

    // did it work?
    if ( ss.fail() )
    {
        raiseError("Failed to parse argument as int");
        result = 0;
    }

    // give it back regardless of result
    return result;
}

void TextFileParser::tokenizeCommandString( const std::string& input,
                                            std::vector<std::string>& output,
                                            bool allowEmpty )
{
    return StringUtils::tokenize( input, ' ', output, allowEmpty );
}

void TextFileParser::tokenizeCommandString( const std::string& input,
                                            const char delim,
                                            std::vector<std::string>& output,
                                            bool allowEmpty )
{
    return StringUtils::tokenize( input, delim, output, allowEmpty );
}

void TextFileParser::raiseError( const std::string& message )
{
    m_hasErrors = true;

    if ( m_errorText.empty() )
    {
        m_errorText += std::string("\n") + message;
    }
    else
    {
        m_errorText = message;
    }
}

/////////////////////////////////////////////////////////////////////////////
// Obj parser
/////////////////////////////////////////////////////////////////////////////
class ObjParser : public TextFileParser
{
public:
    ObjParser();
    virtual ~ObjParser();
    virtual bool parse( std::istream& input );
    ObjData* getParsedObjData();
    std::string dump() const;

protected:
    virtual void reset();

private:
    void processVertexPosition( const std::vector<std::string>& tokens );
    void processVertexTexCoord( const std::vector<std::string>& tokens );
    void processVertexNormal( const std::vector<std::string>& tokens );
    void processGroup( const std::vector<std::string>& tokens );
    void processMaterial( const std::vector<std::string>& tokens );
    void processMaterialLib( const std::vector<std::string>& tokens );
    size_t processFace( const std::vector<std::string>& tokens );

    

private:
    ObjData *m_objdata;
};

ObjParser::ObjParser()
    : TextFileParser( "#" ),
      m_objdata( NULL )
{
}

ObjParser::~ObjParser()
{
    delete m_objdata;
}

bool ObjParser::parse( std::istream& input )
{
    reset();

    // Read each line of the .obj file
    while ( readNextLine( input ) && !hasErrors() )
    {
        std::cout << "LINE '" << lineText() <<  "'" << std::endl;

        // Skip lines that are empty
        if ( isCurrentLineEmpty() )
        {
            continue;
        }

        // Split the input line into "words' that are separated by
        // spaces. The first word is the next obj file command, the others
        // that follow are arguments to that command
        std::vector<std::string> tokens;

        tokenizeCommandString( lineText(), tokens );
        assert( tokens.size() > 0 );

        // What kind of command is this?
        if ( tokens[0] == "v" )
        {
            processVertexPosition( tokens );
        }
        else if ( tokens[0] == "vt" )
        {
            processVertexTexCoord( tokens );
        }
        else if ( tokens[0] == "vn" )
        {
            processVertexNormal( tokens );
        }
        else if ( tokens[0] == "g" )
        {
            processGroup( tokens );
        }
        else if ( tokens[0] == "f" )
        {
            processFace( tokens );
        }
        else if ( tokens[0] == "usemtl" )
        {
            processMaterial( tokens );
        }
        else if ( tokens[0] == "mtllib" )
        {
            processMaterialLib( tokens );
        }
        else if ( tokens[0] == "s" )
        {
            // ignore smoothing groups
        }
        else
        {
            raiseError("Unknown .obj command encountered");
            continue;
        }
    }

    // Make sure the last group has at least one face in it
    if ( m_objdata->currentGroup.empty() == false )
    {
        if ( m_objdata->groups[ m_objdata->currentGroup ].count == 0 )
        {
            raiseError("The last active group didn't have any faces");
        }
    }

    // Now we're done, make sure to return if the parsing was successful or
    // not
    return !hasErrors();
}

void ObjParser::reset()
{
    TextFileParser::reset();

    delete m_objdata;
    m_objdata = new ObjData;
}

void ObjParser::processVertexPosition( const std::vector<std::string>& tokens )
{
    if ( tokens.size() != 4 )
    {
        raiseError("v command requires three arguments");
        return;
    }

    float x = parseFloat( tokens[1] );
    float y = parseFloat( tokens[2] );
    float z = parseFloat( tokens[3] );

    if (! hasErrors() )
    {
        m_objdata->positions.push_back( Vec3( x, y, z ) );
    }
}

void ObjParser::processVertexTexCoord( const std::vector<std::string>& tokens )
{
    if ( tokens.size() != 3 )
    {
        raiseError("vt command requires three arguments");
        return;
    }

    float u = parseFloat( tokens[1] );
    float v = parseFloat( tokens[2] );

    if (! hasErrors() )
    {
        m_objdata->texcoords.push_back( Vec2( u, v ) );
    }
}

void ObjParser::processVertexNormal( const std::vector<std::string>& tokens )
{
    if ( tokens.size() != 4 )
    {
        raiseError("vn command requires three arguments");
        return;
    }

    float x = parseFloat( tokens[1] );
    float y = parseFloat( tokens[2] );
    float z = parseFloat( tokens[3] );

    if (! hasErrors() )
    {
        m_objdata->normals.push_back( Vec3( x, y, z ) );
    }
}

void ObjParser::processGroup( const std::vector<std::string>& tokens )
{
    if ( tokens.size() != 2 )
    {
        raiseError("g requires one argument");
    }

    // Are we currently in a obj group, or this is the first one?
    if ( m_objdata->currentGroup.empty() )
    {
        assert( m_objdata->groups.size() == 0 );

        // Make sure we haven't added any faces yet, otherwise the
        // group will be missing faces
        if ( m_objdata->faces.size() > 0 )
        {
            raiseError("g cannot be called after faces have started");
            return;
        }

        // Create a new group, and add it to the list
        ObjGroup group;

        group.name   = tokens[1];
        group.offset = 0;

        m_objdata->currentGroup = tokens[1];
        m_objdata->groups.push_back( group );
    }
    else
    {
        assert( m_objdata->groups.size() > 0 );

        // There's already a group in use. Make sure it has at least one
        // face (empty groups don't make sense...), finish it off and add
        // the new group
        if ( m_objdata->groups[m_objdata->currentGroup].count == 0 )
        {
            raiseError("Last group didn't have any faces in it");
            return;
        }

        // Now create and add a new group
        ObjGroup group;

        group.name   = tokens[1];
        group.offset = m_objdata->faces.size();

        m_objdata->currentGroup = tokens[1];
        m_objdata->groups.push_back( group );
    }
}

void ObjParser::processMaterial( const std::vector<std::string>& tokens )
{
    // Make sure we have an active group, because it doesn't make sense to
    // attach a material to an non-existance group...
    if ( m_objdata->currentGroup.empty() )
    {
        raiseError("Cannot set material because there are no active groups");
    }
    else if ( m_objdata->groups[ m_objdata->currentGroup ].material.empty() )
    {
        raiseError("Cannot set material on group when one is already set");
    }
    else
    {
        m_objdata->groups[ m_objdata->currentGroup ].material = tokens[1];
    }
}

void ObjParser::processFace( const std::vector<std::string>& tokens )
{
    // Right now we only support faces with three vertices
    if ( tokens.size() != 4 )
    {
        raiseError("f requires three arguments");
    }

    // Parse out each component of the face. For the moment we only accept
    // three part faces, but we can always extend the parser to handle
    // arbitrarily sized polygons
    Face f;

    f.a = processFaceComponent( tokens[1] );
    f.b = processFaceComponent( tokens[2] );
    f.c = processFaceComponent( tokens[3] );

    // Make sure there were no errors in parsing the face command before
    // we insert it
    if (! hasErrors() )
    {
        objdata->faces.push_back( f );
    }
}

void ObjParser::processMaterialLib( const std::string& tokens )
{
    std::string materialfile;
    
    if ( tokens.size() > 1 )
    {
        materialfile = tokens[1];
    }

    std::cerr
        << "* Ignoring material library '" << materialfile
        << "' import. Make sure referenced materials are in the "
        << "proper .material file and remove this line from the obj file"
        << std::endl;
}

size_t ObjParser::processFaceComponent( const std::string& facestr )
{
    // Parse out the components of the face
    std::vector<std::string> parts;
    parseCommandString( facestr, '/', faceParts );

    bool hasPosition = ( parts.size() > 0 && (!parts[0].empty()) );
    bool hasTexCoord = ( parts.size() > 1 && (!parts[1].empty()) );
    bool hasNormal   = ( parts.size() > 2 && (!parts[2].empty()) );

    // Make sure the face format is expected. If it doesn't match to
    // what the previous faces have had, then we need to bail out
    if ( m_objdata->hasDeterminedFormat )
    {
        if ( m_objdata->hasPosition != hasPosition ||
             m_objdata->hasTexCoord != hasTexCoord ||
             m_objdata->hasNormal   != hasNormal )
        {
            raiseError("Face has differing components from previous");
            return 0;
        }
    }
    else
    {
        m_objdata->hasDeterminedFormat = true;

        // They must always have a position component
        if (! hasPosition )
        {
            raiseError("Face is missing required position offset");
            return 0;
        }

        // Record the format for posterity and later validation
        m_objdata->hasPosition = hasPosition;
        m_objdata->hasTexCoord = hasTexCoord;
        m_objdata->hasNormal   = hasNormal;
    }

    // Convert this into a colesced vertex containing the position,
    // texture and normal
    ObjVertex v;

    if ( hasPosition )
    {
        size_t offset = parseInt( parts[0] );

        if ( offset < 0 || offset >= m_objdata->positions.size() )
        {
            raiseError("Invalid position offset");
        }
        else
        {
            v.pos = m_objdata->positions[offset];
        }
    }

    if ( hasTexCoord )
    {
        size_t offset = parseInt( parts[1] );

        if ( offset < 0 || offset >= m_objdata->texcoords.size() )
        {
            raiseError("Invalid texcoord offset");
        }
        else
        {
            v.tex = m_objdata->texcoords[offset];
        }
    }

    if ( hasNormal )
    {
        size_t offset = parseInt( parts[2] );

        if ( offset < 0 || offset >= m_objdata->normals.size() )
        {
            raiseError("Invalid normal offset");
        }
        else
        {
            v.normal = m_objdata->normals[offset];
        }
    }

    // Now look up the colaesced vertex. If it already exists in the
    // vertex cache (and the option to de-duplicated vertices is on) then
    // we'll reference it. Otherwise we'll have to create a new vertex and
    // insert it into the cache
    if (! m_objdata->vertexCache.find( facestr ) )
    {
        // Add the vertex, record its position
        size_t offset = m_objdata->vertices.size();
        m_objdata->vertices.push_back( v );

        // Add vertex to cache
        m_objdata->vertexCache[ facestr ] = offset;
    }

    // Increment the face count for the active group (if there is one)
    if ( m_objdata->currentGroup.empty() == false )
    {
        m_objdata->groups[ m_objdata->currentGroup ].count += 1;
    }

    // And return the vertex offset
    return m_objdata->vertexCache[facestr];
}

std::string ObjParser::dump() const
{
    std::stringstream output;

    output << "===== VERTEX POSITIONS =====\n";

    for ( size_t i = 0; i < m_objdata->positions.size(); ++i )
    {
        output << " " << i << ": "
            << m_objdata->positions[i].x << ", "
            << m_objdata->positions[i].y << ", "
            << m_objdata->positions[i].z << "\n";
    }

    output << "===== VERTEX TEXCOORDS =====\n";

    for ( size_t i = 0; i < m_objdata->texcoords.size(); ++i )
    {
        output << " " << i << ": "
            << m_objdata->texcoords[i].x << ", "
            << m_objdata->texcoords[i].y << "\n";
    }

    output << "===== VERTEX NORMALS =====\n";

    for ( size_t i = 0; i < m_objdata->normals.size(); ++i )
    {
        output << " " << i << ": "
            << m_objdata->normals[i].x << ", "
            << m_objdata->normals[i].y << ", "
            << m_objdata->normals[i].z << "\n";
    }

    return output.str();
}

void loadObj( const std::string& filename )
{
    ObjParser parser;

    if (! parser.parseFile( filename ) )
    {
        std::cerr
            << "Failed to load .obj file: " << parser.errorText() << std::endl
            << "FILE: " << parser.filename()   << ":"
                        << parser.lineNumber() << std::endl
            << "LINE: " << parser.lineText()   << std::endl;
    }
    else
    {
        std::cout << parser.dump();
    }
}


/*
 * Copyright (C) 2011 Scott MacDonald. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "loaders/objparser.h"
#include "math/vector.h"

#include <string>
#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>

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
        if ( m_objdata->groups[ m_objdata->currentGroupIndex ].count == 0 )
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

        m_objdata->currentGroup      = tokens[1];
        m_objdata->currentGroupIndex = m_objdata->groups.size();
        m_objdata->groups.push_back( group );
    }
    else
    {
        assert( m_objdata->groups.size() > 0 );

        // There's already a group in use. Make sure it has at least one
        // face (empty groups don't make sense...), finish it off and add
        // the new group
        if ( m_objdata->groups[m_objdata->currentGroupIndex].count == 0 )
        {
            raiseError("Last group didn't have any faces in it");
            return;
        }

        // Now create and add a new group
        ObjGroup group;

        group.name   = tokens[1];
        group.offset = m_objdata->faces.size();

        m_objdata->currentGroup      = tokens[1];
        m_objdata->currentGroupIndex = m_objdata->groups.size();
        m_objdata->groups.push_back( group );
    }
}

void ObjParser::processMaterial( const std::vector<std::string>& tokens )
{
    // Make sure we have an active group, because it doesn't make sense to
    // attach a material to an non-existance group...
    size_t index = m_objdata->currentGroupIndex;

    if ( m_objdata->currentGroup.empty() )
    {
        raiseError("Cannot set material because there are no active groups");
    }
    else if ( m_objdata->groups[ index ].material.empty() )
    {
        raiseError("Cannot set material on group when one is already set");
    }
    else
    {
        m_objdata->groups[ index ].material = tokens[1];
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
        m_objdata->faces.push_back( f );
    }
}

void ObjParser::processMaterialLib( const std::vector<std::string>& tokens )
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
    tokenizeCommandString( facestr, '/', parts );

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
        int temp      = parseInt( parts[0] );
        size_t offset = static_cast<int>( temp );

        if ( temp < 0 || offset >= m_objdata->positions.size() )
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
        int temp      = parseInt( parts[1] );
        size_t offset = static_cast<int>( temp );

        if ( temp < 0 || offset >= m_objdata->texcoords.size() )
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
        int temp      = parseInt( parts[2] );
        size_t offset = static_cast<int>( temp );

        if ( temp < 0 || offset >= m_objdata->normals.size() )
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
    if ( m_objdata->vertexCache.find( facestr ) != m_objdata->vertexCache.end())
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
        m_objdata->groups[ m_objdata->currentGroupIndex ].count += 1;
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


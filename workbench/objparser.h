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

#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <string>
#include <istream>
#include <vector>
#include <map>

#include "loaders/textfileparser.h"
#include "math/vector.h"
#include "geom/face.h"

struct ObjData;

/**
 * A parser for the .obj 3d mesh file format
 */
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
    void processFace( const std::vector<std::string>& tokens );

    size_t processFaceComponent( const std::string& chunk );

private:
    ObjData *m_objdata;
};

/**
 * Coalesced vertex data for an ObjMesh
 */
struct ObjVertex
{
    Vec3 pos;
    Vec2 tex;
    Vec3 normal;
};

/**
 * Information on a group of faces in an ObjMesh
 */
struct ObjGroup
{
    // Name of the object group
    std::string name;

    // Material used for the group
    std::string material;

    // Where in the face array does this group start
    size_t offset;

    // How many faces does this group contain
    size_t count;
};

/**
 * All the information read out of the .obj file
 */
struct ObjData
{
    ObjData()
        : positions(),
          texcoords(),
          normals(),
          vertices(),
          vertexCache(),
          faces(),
          groups(),
          currentGroup(),
          currentGroupIndex(0),
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

    // Offset of the current group
    size_t currentGroupIndex;

    // Flag if first face has been encountered and specified vertex composition
    bool hasDeterminedFormat;

    // Flag if vertices have position (must be true)
    bool hasPosition;

    // Flag if vertices have tex coordinates
    bool hasTexCoord;

    // Flag if vertices have normal vector
    bool hasNormal;
};

#endif // OBJPARSER_H


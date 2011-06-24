void drawCube()
{
    //
    // Cube vertex positions. Each set of four vertices is arranged in
    // the following order:
    //
    //   A---D
    //   | / |
    //   B---C
    //
    // Also, the six sets are arrange in [+x,-x,+y,-y,+z,-z] order. This
    // means the vertex soup is arranged like so:
    //   1. right face
    //      1. A0 A1 A2
    //      2. B0 B1 B2
    //      3. C 
    //      4. D ...
    //   2. left face
    //   3. back face
    //   4. front face
    //   5. top face
    //   6. bottom face
    //
    // All vertices are specified in OpenGL coordinates, but are ordered
    // along the game's axis (0..6)
    //
    float CubePos[6][4][3] = 
    {   /* right face */
        { { 1.0f, 1.0f, 0.0f },
          { 1.0f, 0.0f, 0.0f },
          { 1.0f, 0.0f,-1.0f },
          { 1.0f, 1.0f,-1.0f } },
        /* left face */
        { { 0.0f, 1.0f,-1.0f },
          { 0.0f, 0.0f,-1.0f },
          { 0.0f, 0.0f, 0.0f },
          { 0.0f, 1.0f, 0.0f } },
        /* back face */
        { { 1.0f, 1.0f,-1.0f },
          { 1.0f, 0.0f,-1.0f },
          { 0.0f, 0.0f,-1.0f },
          { 0.0f, 1.0f,-1.0f } },
        /* front face */
        { { 0.0f, 1.0f, 0.0f },
          { 0.0f, 0.0f, 0.0f },
          { 1.0f, 0.0f, 0.0f },
          { 1.0f, 1.0f, 0.0f } },
        /* top face */
        { { 1.0f, 1.0f, 0.0f },
          { 1.0f, 1.0f,-1.0f },
          { 0.0f, 1.0f,-1.0f },
          { 0.0f, 1.0f, 0.0f } },
        /* bottom face */
        { { 0.0f, 0.0f, 0.0f },
          { 0.0f, 0.0f,-1.0f },
          { 1.0f, 0.0f,-1.0f },
          { 1.0f, 0.0f, 0.0f } }
    };

    /**
     * Array holding the normal vectors for our cube. Vector details
     * are specified in OpenGL space, but are ordered for game axis.
     */
    float CubeNorms[6][3] =
    {
        { 1.0f,  0.0f,  0.0f },
        {-1.0f,  0.0f,  0.0f },
        { 0.0f,  0.0f, -1.0f },
        { 0.0f,  0.0f,  1.0f },
        { 0.0f,  1.0f,  0.0f },
        { 0.0f, -1.0f,  0.0f }
    };


    glBegin( GL_TRIANGLES );
    for ( int k = 0; k < 6; ++k )
    {
        glNormal3fv( CubeNorms[k] );
        
        glVertex3fv( CubePos[k][1] );
        glVertex3fv( CubePos[k][3] );
        glVertex3fv( CubePos[k][0] );

        glVertex3fv( CubePos[k][2] );
        glVertex3fv( CubePos[k][3] );
        glVertex3fv( CubePos[k][1] );
    }
    glEnd();
}

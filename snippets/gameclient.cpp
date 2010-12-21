
class GameClient
{
    void mainloop()
    {
        //
        // Generate the game world
        //   Create( template, ChunkSize, rows, cols, depth )
        //  
        world = WorldFactory::Create( "default", 16, 128, 128, 16 );

        //
        // main game loop
        //
        while ( true )
        {
            // Grab all incoming actions
            while (! actions.empty() )
            {
                Message m = actions.pop();
                executeMessage( m );
            }

            // Handle user input
            procesInput();

            // Render the world
            renderer.renderWorld( world );
        }
    }

    void processInput()
    {
        char c = Keyboard.getButtonPressed();

        switch ( c )
        {
            case 'w':
                actions.add( new MoveAction( playerCharacter, FORWARD ) );
                break;

            case 't':
                Ray r      = translateMouseClickToWorldVector();
                Position p = world.findFirstIntersectionCubeFor( r );

                actions.add( new CubeTake( playerCharacter, START ) );
                break;
        }
    }

    void executeMessage( Message m )
    {
        switch ( m.type )
        {
            case MOVE_ACTION:
                Actor       actor = m.actor;
                CollisionBox  box = actor.getCollisionBox();
                Vector        dir;
                
                // Correctly update movement vector
                if ( m.moveDirection == FORWARD )
                {
                    Vector dir = actor.getForwardVector();
                    dir = somehowMakeVPointFoward( dir );
                }
                else if ( m.moveDirection == BACKWARD )
                {
                    Vector dir = actor.getForwardVector();
                    dir = somehowMakeVPointBackward( dir );
                }

                world.translateBy( dir );

                if (! world.collides( box ) )
                {
                    actor->setPositionRelative( SPEED... )
                }

                break;

            case DESTROY_CUBE:
                Position pos = m.actionPosition;
                ChunkID  cid = world.getChunkIdForPos( pos );

                if (! is_actor_in_range_of_cube( pos ) )
                {
                    break;
                }

                world.destroyCube( c );
                updateGeometryForCube( cid );
                recalculateLightingFor( cid );
                break;

            case ADD_CUBE:
                Position pos = m.actionPosition;
                ChunkId  cid = world.getChunkIdForPos( pos );

                if (! isActorInRangeOf( pos ) )
                {
                    // He's too far away to do this
                    break;
                }

                world.addCube( pos );
                updateGeometryForCube( cid );
                recalculateLightFor( cid );
            }

            case CHUNK_DATA:
                ChunkId cid = m.chunkId;

                if ( world.hasChunk( cid ) )
                {
                    world.destroyChunk( cid );
                }

                world.setChunk( cid );
        }
    }

private:
    // renderer stuff
    ActionList actions;
    Camera     playerCamera;
    Actor      playerCharacter;
};

class Renderer
{
    void renderWorld()
    {
        // Search for all visible world chunks
        ChunkId[] visibleChunks = world.getVisibleChunks( playerCam );
        GeometryData[] geoms;

        // Iterate through the list of visible chunks, and get a list
        // of pointers to their geometry data
        foreach( ChunkId id : visibleChunks )
        {
            GeometryData* data = getCachedChunkOrRebuild( id );
            geoms += data;
        }

        // Now get a list of all camera visible entities. Entitie are
        // basically any physical object in the game world that are NOT
        // themselves cubes. Items, actors, critters etc fall into this
        // list
        EntityId visibleEntities = world.getVisibleEntities( playerCam );

        foreach ( EntityId id : visibleEntities )
        {
            GeometryData* data = getMeshFor( id );
            geoms += data;
        }

        // Sort through the list of visible geometry, and arrange it
        // by material ID to optimize rendering speed
        map<Material, SubMesh[]> items;

        foreach ( GeometryData data : geoms )
        {
            foreach ( GeometrySubData subdata : data.groups )
            {
                items[ subdata ] += subdata.submesh
            }
        }

        // Now render everything
        foreach ( Material m, SubMesh[] meshes : items )
        {
            enableMaterial( m );

            foreach ( SubMesh mesh : meshes )
            {
                draw ( mesh );
            }
        }
    }

    /**
     * Returns a pointer to a chunk's geometric mesh for rendering and
     * other nefarious purposes. This method will look up the chunk id.
     * If the chunk was already built and cached, it will be returned.
     * Otherwise it will have to be compiled before being cached and returned
     */
    GeometryData * getCachedChunkOrRebuild( ChunkId id )
    {
        if ( chunkGeometryData.has(id) )
        {
            // Already cached, return it
            return chunkGeometryData.get( id );
        }
        else
        {
            // Get the actual chunk data from the world storage class
            // Then use the chunk builder class to create a mesh to store
            ChunkData chunk = world.getChunkData( id );
            MeshChunkBuilder builder( chunk );
            GeometryData * geom = builder.compile();

            // Cache the compiled mesh
            chunkGeometryData.add( id, geom );
            return geoms;
        }
    }

private:
    // renderer stuff
    std::map<ChunkId, GeometryData*> chunkGeometryData;
};


class World
{

    void addCube( Position p, CubeType t )
    {
        ChunkId id = getChunkFor( p );
        ChunkData * data = getChunk( id );

        data->setCube( p, t );
    }

    void destroyCube( Position p )
    {
        ChunkId id       = getChunkFor( p );
        ChunkData * data = getChunk( id );

        data->destroyCube( p );
    }

private:
    void getChunkFor( Position p )
    {
        complex magic;
    }

    ChunkData chunks[];
};

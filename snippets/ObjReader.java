package scott.assettool;

import java.io.*;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Scanner;

import scott.math.*;
import scott.geometry.*;

public class ObjReader implements IFileParser
{
    public ObjReader()
    {
        m_groups            = new ArrayList<String>( 8 );
        m_materialMap       = new Hashtable<String, String>();
        m_materialLibraries = new ArrayList<String>();
        m_positions         = new ArrayList<Vector3>( 1024 );
        m_texcoords         = new ArrayList<TexCoord>( 1024 );
        m_normals           = new ArrayList<Vector3>( 1024 );
    }

    public void read( FileReader input )
            throws IOException, ImportException
    {
        Scanner scanner = new Scanner( input );

        while ( scanner.hasNextLine() )
        {
            processLine( scanner );
        }
    }

    /**
     * Process a line in an .obj file
     * @param scanner
     */
    private void processLine( Scanner scanner )
            throws ImportException
    {
        String cmd = scanner.next();

        //
        // Skip empty space and comments
        //
        if ( cmd.startsWith("#") )
        {
            scanner.nextLine();         // consume rest of line
            System.err.println("Comment");
            return;
        }

        //
        // Big long nasty if-elseif statement. Triggers the appropriate
        // per-line handling of an .obj command
        //
        if ( cmd.equals("v") )
        {
            processVertexCmd( scanner );
        }
        else if ( cmd.equals("vt" ) )
        {
            processVTexCmd( scanner );
        }
        else if ( cmd.equals( "vn") )
        {
            processVNormCmd( scanner );
        }
        else if ( cmd.equals( "f" ) )
        {
            processFaceCmd( scanner );
        }
        else if ( cmd.equals( "g" ) )
        {
            processGroupCmd( scanner );
        }
        else if ( cmd.equals( "mtllib") )
        {
            processMtlLibCmd( scanner);
        }
        else if ( cmd.equals( "usemtl") )
        {
            processMatCmd( scanner);
        }
        else
        {
            throw new ImportException("Unknown command: " + cmd );
        }
    }

    /**
     * Process a vertex position
     */
    private void processVertexCmd( Scanner scanner )
    {
        float x = scanner.nextFloat();
        float y = scanner.nextFloat();
        float z = scanner.nextFloat();

        m_positions.add( new Vector3( x, y, z ) );
    }

    /**
     * Process a vertex texture coordinate
     */
    private void processVTexCmd( Scanner scanner )
    {
        float u = scanner.nextFloat();
        float v = scanner.nextFloat();

        m_texcoords.add( new TexCoord( u, v) );
    }

    /**
     * Process a vertex normal component
     */
    private void processVNormCmd( Scanner scanner )
    {
        float x = scanner.nextFloat();
        float y = scanner.nextFloat();
        float z = scanner.nextFloat();

        m_normals.add( new Vector3( x, y, z ) );
    }

    /**
     * Process the big nasty vertex face
     */
    private void processFaceCmd( Scanner scanner )
            throws ImportException
    {
        // Check for an active face group. If there isn't one, create a default
        if ( m_activeGroup == null )
        {
            m_activeGroup = DefaultGroupName;
            m_groups.add( m_activeGroup );
            m_materialMap.put( m_activeGroup, DefaultMaterialName );
        }
        

        // Consume all face blocks. This is a pretty messy solution, but
        // we need to deal with multiple groups and differing formatting
        // depending on the obj file
        String contents = scanner.nextLine().trim();
        String[] chunks = contents.split("\\s+");
        int[][] face    = new int[4][3];

        //
        // Sanity checks - make sure they are of correct size
        //
        if ( chunks.length > 4 )
        {
            throw new ImportException( "Refusing to parse face group with " +
                    "more than four vertices: '" + contents + "'" );
        }
        else if ( chunks.length < 3 )
        {
            throw new ImportException( "Cannot parse face group with less " +
                    "than three vertices: " + contents );
        }

        //
        // Split the chunks up into (pos/tex/normal)
        //
        int pos = -1, tex = -1, nrm = -1;

        for ( String chunk : chunks )
        {
            String[] bits = chunk.split( "/" );

            // Grab each part of the component description, make sure they are
            // there and that they are correct
            if ( bits.length == 1 )
            {
                // only position...
                pos = Integer.parseInt( chunk );

                SetOrVerifyFaceComposition( contents, false, false );
            }
            else if ( bits.length == 2 )
            {
                // position + tex coord
                pos = Integer.parseInt( bits[0] );
                tex = Integer.parseInt( bits[1] );

                SetOrVerifyFaceComposition( contents, true, false );
            }
            else if ( bits.length == 3 )
            {
                // could have tex coord and or normal
                //  (SUPER annoying variations... sigh)
                boolean bTexCoord = (bits[1] == null);
                boolean bNormal   = (bits[2] == null);

                pos = Integer.parseInt( bits[0] );
                
                if ( bTexCoord ) { tex = Integer.parseInt( bits[1] ); }
                if ( bNormal   ) { nrm = Integer.parseInt( bits[2] ); }

                SetOrVerifyFaceComposition( contents, bTexCoord, bNormal );
            }

            // Quick debug print
//            System.out.println( "p: " + pos + ", t: " + tex + ", n: " +
//                    nrm );

            // Assign it
        }

        //
        // Add the face to our list of faces
        //
        if ( chunks.length == 3 )
        {
        }
        else
        {
            // Four faces... make sure to split it correctly
        }
    }

    /**
     * Process a "set active group name" command
     */
    private void processGroupCmd( Scanner scanner )
    {
        String groupName = scanner.next();

        // Set active group
        m_activeGroup = groupName;

        // Is this group registered in the list of groups?
        // If not, register it
        if (! m_groups.contains( groupName ) )
        {
            m_groups.add( groupName );
            m_materialMap.put( groupName, DefaultMaterialName );
        }
    }

    /**
     * Process specification of an external materials library
     */
    private void processMtlLibCmd( Scanner scanner )
    {
        String filename = scanner.next();

        // Add it to the list of material libraries
        m_materialLibraries.add(  filename );
    }

    /**
     * Process a "use material on active group" command
     */
    private void processMatCmd( Scanner scanner )
    {
        String materialName = scanner.next();

        // Check for an active face group. If there isn't one, create a default
        if ( m_activeGroup == null )
        {
            m_activeGroup = DefaultGroupName;
            m_groups.add( m_activeGroup );
            m_materialMap.put( m_activeGroup, materialName );
        }
        else
        {
            // Remove the old material if there was one
            if ( m_materialMap.contains( m_activeGroup ) )
            {
                m_materialMap.remove( m_activeGroup );
            }

            // Add it
            m_materialMap.put( m_activeGroup, materialName );
        }
    }

    /**
     * Helper method that verifies .obj face composition. This method will
     * check that the given composition (has position, has texture coordinate,
     * has normal) matches previous faces in the .obj file.
     *
     * If this is the first face command, it will set the precedent for all
     * faces to follow.
     *
     * If the verifcation fails, it will throw a fatal exception notifying the
     * user that the .obj file has an incorrect face command.
     *
     * @param line      Contents of the line being verified
     * @param texcoord  Flag specifying if a texture coordinate was given
     * @param normal    Flag specifying if a normal coordinate was given
     * @throws ImportException
     */
    private void SetOrVerifyFaceComposition(
            String line,
            boolean texcoord,
            boolean normal )
       throws ImportException
    {
        if ( m_firstFaceGroup )
        {
            m_hasTexCoord = texcoord;
            m_hasNormal   = normal;
        }
        else if ( m_hasTexCoord != texcoord || m_hasNormal != normal )
        {
            throw new ImportException( "Face command is invalid. It specifies "+
                    "a differing of texcoord or normal: " + line );
        }
    }

    private boolean m_firstFaceGroup = true;
    private boolean m_hasTexCoord    = false;
    private boolean m_hasNormal      = false;
    private String  m_activeGroup   = null;

    /**
     * List of groups specified in .obj file
     */
    private ArrayList<String>   m_groups;

    /**
     * List of referenced material files
     */
    private ArrayList<String>   m_materialLibraries;

    /**
     * Mapping of face groups to material name
     */
    private Hashtable<String, String> m_materialMap;

    /**
     * List of positions specified in the .obj file
     */
    private ArrayList<Vector3>  m_positions;

    /**
     * List of texture coordinates specified in the .obj file
     */
    private ArrayList<TexCoord> m_texcoords;

    /**
     * List of normals specified in the the .obj file
     */
    private ArrayList<Vector3>  m_normals;

    private final static String DefaultGroupName    = "Root";
    private final static String DefaultMaterialName = "DefaultMaterial";
}

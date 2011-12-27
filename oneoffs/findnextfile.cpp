/**
 * Given a filename template, looks for the next name to use
 * in sequence in a directory.
 *
 * eg, given findNextAvailableName( FILENAME_, 3 ) will return
 * something like:
 *   FILENAME_000
 *   FILENAME_099
 *   etc
 */
#include <iostream>
#include <cassert>

#include <sys/types.h>
#include <dirent.h>
#include <cstdlib>

#define raiseError(x) appRaiseError(__FILE__,__func__,x,__LINE__)

void appRaiseError( const std::string& filename,
                    const std::string& function,
                    const std::string& message,
                    int line )
{
    std::cerr << "Application encountered fatal error in file "
              << filename << ", line " << line << " ("
              << function << ")."
              << std::endl;
    std::cerr << "MESSAGE: " << message << std::endl;
    exit(1);
}

/**
 * Checks if filename is of pattern
 *  <matchname>NNN<matchext>
 *
 *  Where NNN is one or more numeric digits
 */
bool checkFilenameSetMatches( const std::string& filename,
                              const std::string& matchname,
                              const std::string& matchext,
                              int& setIndex )
{
    size_t nameMatches   = 0;
    size_t extMatches    = 0;
    size_t numberMatches = 0;

    size_t fpos = 0;

    //
    // Check if the filename matches
    //
    for ( size_t i = 0 ;
          i < matchname.size() && fpos < filename.size();
          ++i, ++fpos )
    {
        if ( filename[fpos] == matchname[i] )
        {
            nameMatches++;
        }
        else
        {
            break;
        }
    }

    //
    // Now see if there is a number afterward
    //
    size_t numberStartPos  = fpos;

    for ( size_t i = 0;
          fpos < filename.size();
          ++i, ++fpos )
    {
        // Check if the character is actually a number
        if ( isdigit( filename[fpos] ) )
        {
            numberMatches++;
        }
        else
        {
            // Unless it's a file extension '.', then it is invalid
            if ( filename[fpos] != '.' )
            {
                numberMatches = 0;
            }
            else
            {
                // Advance to next character, and match the file ext
                fpos++;
            }

            break;
        }
    }

    //
    // Now see if the extension matches
    //
    for ( size_t i = 0;
          i < matchext.size() && fpos < filename.size();
          ++i, ++fpos )
    {
        if ( matchext[i] == filename[fpos] )
        {
            extMatches++;
        }
        else
        {
            break;
        }
    }

    bool result = ( (nameMatches == matchname.size()) &&
                    (extMatches  == matchext.size() ) &&
                     numberMatches > 0 );

    //
    // If the pattern matched, extract the fileset id
    //
    if ( result )
    {
        std::string num = filename.substr( numberStartPos, numberMatches );
        setIndex        = atoi( num.c_str() );
    }

    return result;
}

std::string findNextAvailableFilename( const std::string& dirpathname,
                                       const std::string& filename,
                                       const std::string& fileext,
                                       int digits )
{
    fs::path dirpath = dirpathname;

    

    //
    // Open the requested directory, and iterate all of the files
    // in it
    //
    dirent *dent = NULL;
    DIR    *dir  = opendir( dirpath.c_str() );

    if ( dir == NULL )
    {
        raiseError("Failed to open directory " + dirpath);
    }

    //
    // Iterate files
    //
    int largestIndex = 0;

    while ( (dent = readdir(dir)) )
    {
        int fileSetIndex = 0;
        std::string currentName = std::string( dent->d_name );

        if ( checkFilenameSetMatches( currentName, filename, fileext,
                                      fileSetIndex ) )
        {
            std::cout << "FOUND: " << currentName << std::endl;

            // Is this the largest set index found so far?
            if ( fileSetIndex > largestIndex )
            {
                largestIndex = fileSetIndex;
            }
        }
    }

    //
    // report it
    //
    std::cout << "LARGEST INDEX WAS: " << largestIndex << std::endl;

    //
    // Clean up
    //
    closedir( dir );

    //
    // Return the new name
    //
    digits = digits;    // NOT DONE
    return "NOTDONE";
}


int main( int , char**  )
{
    findNextAvailableFilename("files", "base_", "txt", 3 );
}

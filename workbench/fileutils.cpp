/**
 * Copyright 2010 Scott MacDonald. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY SCOTT MACDONALD ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL <COPYRIGHT HOLDER> OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Scott MacDonald.
 */

/////////////////////////////////////////////////////////////////////////////
// HEADER
////////////////////////////////////////////////////////////////////////////
#ifndef SCOTT_WORKBENCH_FILE_UTILS_H
#define SCOTT_WORKBENCH_FILE_UTILS_H
#define FILE_UTILS_VERSION 2

#include <iosfwd>
#include <fstream>
#include <string>
#include <vector>

#ifndef TYPEDEF_STRING_ARRAY
#define TYPEDEF_STRING_ARRAY
typedef std::vector<std::string> StringArray;
#endif

bool fileExists( const std::string& path );

time_t getFileTimestamp( const std::string& path );

size_t getFileSize( const std::string& path );

/**
 * Converts the relative filepath to an absolute system path
 */
std::string convertToAbsolutePath( const std::string& path );

/**
 * Returns a list of directories comprising a file path
 *
 * The first element is always the topmost (closest to root or /), and
 * the last element is always the path immediately proceeding the file.
 *
 * If the file path contains no directories, the returned vector shall be
 * empty.
 *
 * ex: "/path/to/this/file.extension" return [path,to,this]
 *
 * (TODO how to handle relative dirs like . and ..)
 */
std::vector<std::string> parseDirsFromPath( const std::string& path );

/**
 * Extracts and returns a filename from its path. So, it a file path looks
 * like "/path/to/this/file.extension" the result will be "file"
 *
 * If the filename contains multiple ., then they will all be part of the
 * file except for the last one. ("this.is.a.file" return "this.is.a")
 */
std::string getBaseFilename( const std::string& path );

/**
 * Extracts and returns the directory from the given path.
 *
 * ex: "/path/to/this/file.extension" returns "/path/to/this/file"
 * (trailing / ALWAYS omitted unless root)
 */
std::string getDirectoryFromPath( const std::string& path );

/**
 * Extracts and returns the file's extension from the given path. This will
 * always be the last .XXX in the file
 *
 * ex: "/path/to/this/file.blah.extension" return "extension"
 */
std::string getFileExtensionFromPath( const std::string& path );

std::string stripFinalSlash( const std::string& path );

StringArray findInDirectory( const std::string& path,
                             const std::string& pattern,
                             bool recurse = false,
                             bool ignoreFiles    = false,
                             bool ignoreDirs     = false,
                             bool ignoreSymlinks = true,
                             bool ignoreSpecials = true );

StringArray findFilesInDirectory( const std::string& path, bool recurse=false )
 ;

bool deleteFile( const std::string& path );

bool createDirectory( const std::string& path );

std::string getNextUnusedFilename( const std::string& basename,
                                   const std::string& path );

std::string getCurrentDirectory();

void setCurrentDirectory();

/**
 * Checks the equvilancy of two files. File1 is the file from path1,
 * and file2 is the file from path2.
 *
 *   file1 < file2     returns -1
 *   file1 = file2     returns  0
 *   file1 > file2     returns  1
 */
int compareFiles( const std::string& path1, const std::string& path2 );

bool isDirectory( const std::string& path );

bool isFile( const std::string& path );

bool copyFile( const std::string& from, const std::string& to );

/**
 * Write a text file to disk
 */
bool loadFileAsStringArray( const std::string& path,
                                  std::vector<std::string>& lines );

bool loadFile( const std::string& path, std::string& contents );

/**
 * Takes a string, and writes it to a file on disk specified by path.
 * Returns false if there was an error (file didn't exist, bad perms,
 * etc). This function will write the string out in 'text' mode.
 */
bool writeFile( const std::string& path, const std::string& contents );

/**
 * Takes an array of bytes (with length 'size'), and writes it to a file
 * on disk specified by path. Returns false if there was an error. This
 * function will write the data in 'binary' mode.
 */
bool writeFile( const std::string& path, const char* data, size_t size );

/**
 * Takes an array of strings, and writes it to a file on disk specified
 * by path. Note that each line will be written with a newline following
 * it (in text mode).
 *
 * Returns false if there was an error
 */
bool writeStringArrayToFile( const std::string& path,
                             const std::vector<std::string>& contents );

#endif

/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include <cassert>


#include <sys/stat.h>

bool fileExists( const std::string& path )
{
    return path == path;
}

time_t getFileTimestamp( const std::string& path )
{
    path == path;
    return time_t();
}

size_t getFileSize( const std::string& path )
{
    return path.size();
}

/**
 * Converts the relative filepath to an absolute system path
 */
std::string convertToAbsolutePath( const std::string& path )
{
    return path;
}

/**
 * Returns a list of directories comprising a file path
 *
 * The first element is always the topmost (closest to root or /), and
 * the last element is always the path immediately proceeding the file.
 *
 * If the file path contains no directories, the returned vector shall be
 * empty.
 *
 * ex: "/path/to/this/file.extension" return [path,to,this]
 *
 * (TODO how to handle relative dirs like . and ..)
 */
std::vector<std::string> parseDirsFromPath( const std::string& path )
{
    path == path;
    return std::vector<std::string>();
}

/**
 * Extracts and returns a filename from its path. So, it a file path looks
 * like "/path/to/this/file.extension" the result will be "file"
 *
 * If the filename contains multiple ., then they will all be part of the
 * file except for the last one. ("this.is.a.file" return "this.is.a")
 */
std::string getBaseFilename( const std::string& path )
{
    return path;
}

/**
 * Extracts and returns the directory from the given path.
 *
 * ex: "/path/to/this/file.extension" returns "/path/to/this/file"
 * (trailing / ALWAYS omitted unless root)
 */
std::string getDirectoryFromPath( const std::string& path )
{
    return path;
}

/**
 * Extracts and returns the file's extension from the given path. This will
 * always be the last .XXX in the file
 *
 * ex: "/path/to/this/file.blah.extension" return "extension"
 */
std::string getFileExtensionFromPath( const std::string& path )
{
    return path;
}

std::string stripFinalSlash( const std::string& path )
{
    return path;
}

StringArray findInDirectory( const std::string& path,
                             const std::string& pattern,
                             bool recurse,
                             bool ignoreFiles,
                             bool ignoreDirs,
                             bool ignoreSymlinks,
                             bool ignoreSpecials )
{
    path == path; pattern == pattern;
    recurse = ignoreFiles = ignoreDirs = ignoreSymlinks = ignoreSpecials;
    recurse = recurse;
    return StringArray();
}

StringArray findFilesInDirectory( const std::string& path, bool recurse )
{
    return findInDirectory( path,
                            std::string(),
                            recurse,
                            true,
                            false,
                            true,
                            true
    );
}

bool deleteFile( const std::string& path )
{
    path == path;
    return false;
}

bool createDirectory( const std::string& path )
{
    return mkdir( path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR );
}

std::string getNextUnusedFilename( const std::string& basename,
                                   const std::string& path )
{
    return basename + path;
}

std::string getCurrentDirectory()
{
    return std::string("omg");
}

void setCurrentDirectory()
{

}

/**
 * Checks the equvilancy of two files. File1 is the file from path1,
 * and file2 is the file from path2.
 *
 *   file1 < file2     returns -1
 *   file1 = file2     returns  0
 *   file1 > file2     returns  1
 */
int compareFiles( const std::string& path1, const std::string& path2 )
{
    return path1 == path2;
}

bool isDirectory( const std::string& path )
{
    return path.size() > 0;
}

bool isFile( const std::string& path )
{
    return path.size() > 0 ;
}

bool copyFile( const std::string& from, const std::string& to )
{
    return from == to;
}

bool loadFileAsStringArray( const std::string& path,
                                  std::vector<std::string>& lines )
{
    std::ifstream file( path.c_str(), std::ios::in );

    if (! file )
    {
        return false;
    }

    //
    // Keep reading lines from the file until there are no more lines
    // to be read. Add each one to the vector
    //
    std::string line;

    while ( file.good() )
    {
        std::getline( file, line );

        if ( line.size() > 0 )      // EOF after \n adds blank line
        {
            lines.push_back( line );
        }
    }

    // 
    // Clean up
    //
    file.close();
    return true;
}

bool loadFile( const std::string& path, std::string& contents )
{
    std::ifstream file( path.c_str(), std::ios::in );

    if (! file )
    {
        return false;
    }

    //
    // Keep reading lines from the file until there are no more lines
    // to be read. Add each one to the vector
    //
    std::stringstream ss;
    std::string line;

    while ( file.good() )
    {
        std::getline( file, line );

        if ( line.size() > 0 )      // EOF after \n triggers blank line
        {
            ss << line << "\n";
        }
    }

    // Assign
    contents = ss.str();

    // Remove the trailing \n
    if ( contents.size() > 0 )
    {
        contents = contents.substr( 0, contents.size() - 1 );
    }

    // 
    // Clean up
    //
    file.close();
    return true;
}

bool writeFile( const std::string& path, const std::string& contents )
{
    std::ofstream file( path.c_str(), std::ios::out | std::ios::trunc );

    if (! file.is_open() )
    {
        return false;
    }

    file << contents;
    file.close();               // That was easy! :O

    return true;
}

bool writeFile( const std::string& path, const uint8_t* data, size_t size )
{
    assert( data != NULL );
    std::ofstream file( path.c_str(), std::ios::out | std::ios::binary );

    if (! file.is_open() )
    {
        return false;
    }

    file.write( (const char*) data, size );
    file.close();

    return true;
}

bool writeStringArrayToFile( const std::string& path,
                             const std::vector<std::string>& contents )
{
    std::ofstream file( path.c_str(), std::ios::out | std::ios::trunc );

    if (! file )
    {
        return false;
    }

    // Write
    for ( size_t i = 0; i < contents.size(); ++i )
    {
        file << contents[i];

        if ( i < contents.size()-1 )
        {
            file << std::endl;      // not sure if this is correct
        }
    }

    // All done!
    file.close();
    return true;
}


/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <googletest/googletest.h>

TEST(FileUtils,NonexistantFileShouldNotExist)
{
    EXPECT_FALSE( fileExists("idonotexist.txt") );
}

TEST(FileUtils,FileExistsActuallyExists)
{
    EXPECT_TRUE( fileExists("iexist.txt") );
}

TEST(FileUtils,TheCurrentMainFunctionThatNeedsToBeRefactored)
{
    std::string filename = "test.txt";
    std::string contents;
    std::vector<std::string> lines;

    if (! loadFile( filename, contents ) )
    {
        std::cerr << "Failed to load " << filename << std::endl;
    }

    if (! loadFileAsStringArray( filename, lines ) )
    {
        std::cerr << "Failed to load " << filename << std::endl;
    }

    std::cout << "----- As string -----" << std::endl;
    std::cout << "'" << contents << "'" << std::endl;

    std::cout << "----- As array -----" << std::endl;
    for ( size_t i = 0; i < lines.size(); ++i )
    {
        std::cout << i << ". '" << lines[i] << "'" << std::endl;
    }


    if (! writeFile( "output_s.txt", contents ) )
    {
        std::cerr << "Failed to write file " << filename << std::endl;
    }

    if (! writeFile( "output_a.txt", contents ) )
    {
        std::cerr << "Failed to write file " << filename << std::endl;
    }
}

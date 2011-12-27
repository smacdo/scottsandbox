#include "archive.h"
#include "thirdparty/sha2.h"
#include "crc.h"
#include "constants.h"

#include <fstream>
#include <stdint.h>
#include <vector>
#include <string>
#include <cassert>

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = boost::filesystem;

/**
 * Archive constructor. Creates an in-memory archive with nothing initially
 * stored in it.
 *
 * \param  archiveName  The name to give to the archive
 */
Archive::Archive( const std::string& archiveName )
    : mArchiveName( archiveName ),
      mHeader(),
      mFileEntries(),
      mErrorMessage()
{
}

/**
 * Archive destructor. Unloads any loaded resources, but will not save a
 * modified archive to disk
 */
Archive::~Archive()
{
    unload();
}

/**
 * Dump information about the current archive file
 */
void Archive::debugDump()
{
    //
    // Instance information
    //
    std::cout
        << "Archive Instance Variables"                      << std::endl
        << "\tmArchiveName        = " << mArchiveName        << std::endl
        << "\tmFileEntries.size() = " << mFileEntries.size() << std::endl
        << "\tmErrorMessage       = " << mErrorMessage       << std::endl
        << std::endl;

    //
    // Header information
    //
    std::cout
        << "HEADER"      << std::endl
        << "\tmagic  : " << std::hex
                         << (int) mHeader.magic[0] << " " << (int) mHeader.magic[1] << " "
                         << (int) mHeader.magic[2] << " " << (int) mHeader.magic[3] << " "
                         << (int) mHeader.magic[4] << " " << (int) mHeader.magic[5] << " "
                         << (int) mHeader.magic[6] << " " << (int) mHeader.magic[7] << " "
                         << std::dec
                         << std::endl
        << "\tversion: " << (int) mHeader.version << std::endl
        << "\tflags  : " << (int) mHeader.archiveFlags   << std::endl
        << "\tentries: " << mHeader.numFileEntries << std::endl
        << "\tentryof: " << mHeader.fileEntryDataOffset << std::endl
        << std::endl;

    std::cout
        << "FILE ENTRIES" << std::endl;

    //
    // Now show all of the files in the archive
    //
    for ( size_t i = 0; i < mFileEntries.size(); ++i )
    {
        std::cout
            << "\tfilename: " << mFileEntries[i].filename() << std::endl
            << "\tmemsize : " << mFileEntries[i].memorySize() << std::endl
            << std::endl;
    }
}

/**
 * Open an archive file from disk
 */
bool Archive::open( const std::string& filename )
{
    // Makes sure to unload the current archive before opening a new one
    unload();

    // Create a binary input file stream that we will use to read the archive's
    // data from
    std::ifstream ifs( filename.c_str(), std::ios::binary | std::ios::in );

    if (! ifs.good() )
    {
        raiseError("Failed to open file for archive reading: " + filename );
        return false;
    }

    // Pull the header in and validate it
    ifs.read( reinterpret_cast<char*>( &mHeader ), sizeof(ArchiveHeader) );
    
    if (! validateHeader() )
    {
        return false;
    }

    // Read the rest of the file into a temporary memory buffer. Once everything
    // is in memory, we can verify the archive integrity and then proceed to
    // reading the file entries.
    const std::size_t headerSize = sizeof(ArchiveHeader);
    const std::size_t faeSize    = sizeof(ArchiveFileEntry);
    const std::size_t tocOffset  = mHeader.fileEntryDataOffset;
    const std::size_t dataSize   = tocOffset - headerSize;
    const std::size_t totalSize  = dataSize + faeSize * mHeader.numFileEntries;

    boost::scoped_array<uint8_t> pArchiveData( new uint8_t[ totalSize ] );

    // Slurp the delicious bits into memory
    ifs.read( reinterpret_cast<char*>(&pArchiveData[0]), totalSize );

    // Set up pointers that will allow us to easily access portions of this
    // archive
    const uint8_t * pFileDataStore          = &pArchiveData[0];
    const ArchiveFileEntry *pArchiveEntries =
        reinterpret_cast<ArchiveFileEntry*>( &pArchiveData[0] + dataSize );

    // Compute the SHA-2 hash digest of the archive data, and compare it to the
    // hash value stored in the archive header. If the values do not match
    // then either the archive is corrupted or someone has tampered with it.

    // Now pull in all of our file entries, and for each one create the
    // appropriate filedata struct referencing the actual binary data
    for ( size_t i = 0; i < mHeader.numFileEntries; ++i )
    {
        // Grab this entry's header from the archive data buffer
        const ArchiveFileEntry& archiveEntry = pArchiveEntries[i];

        assert( archiveEntry.fileOffset    != 0 );
        assert( archiveEntry.fileEntrySize != 0 );

        // Allocate space for the entry's file data, and then copy it from
        // the temporary data store
        std::size_t dataOffset   = archiveEntry.fileOffset - headerSize;
        const uint8_t * pDataPtr = &pFileDataStore[0] + dataOffset;
        uint8_t * pFileData      = new uint8_t[ archiveEntry.fileEntrySize ];

        std::copy( &pFileDataStore[0] + dataOffset,
                   &pFileDataStore[0] + dataOffset + archiveEntry.fileEntrySize,
                   &pFileData[0] );

        // Check that the CRC stored in the archive matches up with a CRC
        // calculate from the data in memory
        uint32_t result = crc32( pFileData, archiveEntry.fileEntrySize );

        if ( result != archiveEntry.checksum )
        {
            std::string filename( archiveEntry.filename );
            raiseError( "CRC32 checksum failed for file " + filename );
        }

        // Add this file entry to our archive so we can refer back to it
        // when the user asks for file data
        FileEntry fileEntry( archiveEntry.filename,
                             pFileData,
                             archiveEntry.fileEntrySize );

        mFileEntries.push_back( fileEntry );
    }

    // Double check if we read the archive file in correctly. Any errors in
    // the stream meant something broke, so we shall report it
    if (! ifs.good() )
    {
        unload();       // just in case
        ifs.close();

        // Need to do clean up here! This leaves us in an very inconsistent
        // state
        raiseError( "Failed while reading archive: " + filename );
        return false;
    }
    else
    {
        ifs.close();
        mArchiveName = filename;
    }

    return true;
}

/**
 * Instructs the archive instance to unload, which will remove all archive
 * entries from memeory and any unsaved changes to be lost.
 */
void Archive::unload()
{
    // Go through all of the loaded file entries, and delete their allocated
    // memory.
    for ( size_t i = 0; i < mFileEntries.size(); ++i )
    {
        boost::checked_array_delete( mFileEntries[i].memoryPointer() );
    }

    // Clear out the header as well
    mHeader.archiveFlags        = 0;
    mHeader.numFileEntries      = 0;
    mHeader.fileEntryDataOffset = 0;

    mFileEntries.clear();
    mArchiveName.clear();
}

/**
 * Closes an archive file, which forces it to unload
 */
void Archive::close()
{
    unload();
}

/**
 * Add a file to the archive. The archive makes a copy the data pointer, so
 * the caller must remember to unallocate the file buffer.
 */
bool Archive::add( const std::string& filename,
                   const uint8_t* pPassedFileData,
                   std::size_t numberOfBytes )
{
    assert( filename.size() > 0 );
    assert( pPassedFileData != NULL );
    assert( numberOfBytes > 0 );

    // Allocate space for the file's data contents, and then make a copy of
    // the file's data before we store it
    uint8_t *pFileData = new uint8_t[ numberOfBytes ];

    std::copy( &pPassedFileData[0], &pPassedFileData[0] + numberOfBytes,
               &pFileData[0] );

    // Add a file entry to the in memory archive and make sure to also update
    // the archive header for consistency
    mFileEntries.push_back( FileEntry( filename, pFileData, numberOfBytes ) );

    mHeader.numFileEntries += 1;
}

/**
 * Removes a file from the archive
 */
bool Archive::remove( const std::string& filename )
{
    // About as simple as deleting it
}

// Check if a file is in the archive
bool Archive::exists( const std::string& filename )
{
}

size_t Archive::calculateFileDataStoreSize() const
{
    size_t totalBytes = 0;

    for ( size_t i = 0; i < mFileEntries.size(); ++i )
    {
        totalBytes += mFileEntries[i].memorySize();
    }

    return totalBytes;
}

/**
 * Returns the number of files contained in this arcihve
 */
size_t Archive::fileCount() const
{
    return mFileEntries.size();
}

/**
 * Return a list of the files contained in this archive
 */
std::vector<std::string> Archive::fileNameList() const
{
    std::vector<std::string> filenames;

    for ( int i = 0; i < mFileEntries.size(); ++i )
    {
        filenames.push_back( mFileEntries[i].filename() ) ;
    }

    return filenames;
}

/**
 * Updates all record data in the archive, and then writes everything out
 * to disk
 */
bool Archive::save()
{
    // Make sure we have an archive name to save out to before we actually
    // start writing
    //   (TODO: If it doesn't end in .far then add it)
    //   (TODO: Actually, mArchiveName shouldn't have .far... just add it)
    assert( mArchiveName.size() > 0 );
    assert( mHeader.numFileEntries == mFileEntries.size() );

    // Calculate the total size that will be required when writing out all
    // of the file entry's file data
    const std::size_t fileDataSize = calculateFileDataStoreSize();

    // Allocate a memory buff to store all the file contents, and then
    // copy each file's data into this memory buffer. Also start creating a
    // list of TOC entries that will be written out
    boost::scoped_array<uint8_t> pFileDataStore(new uint8_t[fileDataSize]);
    std::vector<ArchiveFileEntry> archiveEntries;

    std::size_t headerSize      = sizeof( ArchiveHeader );
    std::size_t fileStoreOffset = 0;

    for ( size_t i = 0; i < mFileEntries.size(); ++i )
    {
        const FileEntry& fileEntry = mFileEntries[i];
        
        // Calculate the CRC32 checksum for this file entry's file data
        uint32_t result = crc32( fileEntry.memoryPointer(),
                                 fileEntry.memorySize() );

        // Generate the file archive entry before copying memory
        ArchiveFileEntry farEntry( fileEntry.filename(),
                                   fileEntry.memorySize(),
                                   fileStoreOffset + headerSize,
                                   result );

        archiveEntries.push_back( farEntry );

        // Now copy the entry's file data into our temporary memory buffer
        // that will be written to disk
        const uint8_t * pFileData = fileEntry.memoryPointer();
        std::copy( &pFileData[0],
                   &pFileData[0] + fileEntry.memorySize(),
                   &pFileDataStore[0] + fileStoreOffset );

        // Move the file store pointer further into the buffer
        fileStoreOffset += fileEntry.memorySize();
    }

    // Update the header's TOC offset to account for the data chunks
    mHeader.fileEntryDataOffset = fileDataSize + headerSize;

    // Create an output filestream that we will use to stream all of te
    // archive data to
    std::ofstream afs( mArchiveName.c_str(),
                       std::ios::binary | std::ios::out );

    if (! afs.good() )
    {
        raiseError("Failed to open file for archive writing: " + mArchiveName );
        return false;
    }

    // First we need to write the file header (Who would've guessed?)
    afs.write( reinterpret_cast<char*>(&mHeader), sizeof(ArchiveHeader) );
    
    // Write out the archive file data
    afs.write( reinterpret_cast<char*>(&pFileDataStore[0]),
               sizeof(uint8_t) * fileDataSize );

    // Write out the archive table of contents. Simply loop through our
    // generated list of file archive structs and dump them to disk
    for ( size_t i = 0; i < archiveEntries.size(); ++i )
    {
        const ArchiveFileEntry& fae = archiveEntries[i];
        afs.write( reinterpret_cast<const char*>( &fae ),
                  sizeof(ArchiveFileEntry) );
    }

    afs.close();
    return true;
}

/**
 * Validate the archive header
 */
bool Archive::validateHeader()
{
    const ArchiveHeader& h  = mHeader;
    const size_t headerSize = sizeof(ArchiveHeader); 
    bool isValid            = true;

    if ( h.magic[0] != 0x89 || h.magic[1] != 0x46 ||
         h.magic[2] != 0x41 || h.magic[3] != 0x52 ||
         h.magic[4] != 0x0D || h.magic[5] != 0x0A ||
         h.magic[6] != 0x1A || h.magic[7] != 0x0A )
    {
        raiseError( "Magic number in archive header is invalid" );
        isValid = false;
    }

    if ( h.version != 1 )
    {
        raiseError( "Archive version not supported" );
        isValid = false;
    }

    if ( h.fileEntryDataOffset < headerSize )
    {
        raiseError( "Incorrect fileEntryDataOffset. Too small" );
        isValid = false;
    }

    if ( (h.numFileEntries >  0 && h.fileEntryDataOffset <= headerSize) ||
         (h.numFileEntries == 0 && h.fileEntryDataOffset != headerSize) )
    {
        raiseError( "Archive file entry and data offset mismatch" );
        isValid = false;
    }

    return isValid;
}

/**
 * Clears any active errors
 */
void Archive::clearErrors()
{
    mErrorMessage = std::string();
}

/**
 * Checks for the existence of an error
 */
bool Archive::hasErrors() const
{
    return (! mErrorMessage.empty() );
}

/**
 * Returns the error message
 */
std::string Archive::errorMessage() const
{
    return mErrorMessage;
}

/**
 * Raises an error
 */
void Archive::raiseError( const std::string& message )
{
    if ( mErrorMessage.empty() )
    {
        mErrorMessage = message;
    }
    else
    {
        mErrorMessage += "\n";
        mErrorMessage += message;
    }
}

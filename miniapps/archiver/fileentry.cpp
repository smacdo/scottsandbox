#include "fileentry.h"

#include <stdint.h>
#include <string>
#include <cassert>

#include <boost/utility.hpp>

FileEntry::FileEntry( const std::string& filename,
                      uint8_t *pFileData,
                      size_t memorySize )
    : mFilename( filename ),
      mMemorySize( memorySize ),
      mpFileMemory( pFileData )
{
    assert( mFilename.size() > 0 );
    assert( mpFileMemory != NULL );
    assert( mMemorySize > 0 );
}

std::string FileEntry::filename() const
{
    return mFilename;
}

std::size_t FileEntry::memorySize() const
{
    return mMemorySize;
}

const uint8_t* FileEntry::memoryPointer() const
{
    return mpFileMemory;
}

void FileEntry::releaseMemory()
{
    boost::checked_array_delete( mpFileMemory );

    mpFileMemory = NULL;
    mMemorySize  = 0;
    mFilename    = "";
}

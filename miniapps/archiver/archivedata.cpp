#include "archivedata.h"
#include "constants.h"
#include "thirdparty/sha2.h"

#include <stdint.h>
#include <string>
#include <cstddef>      // remove?
#include <cstring>      // remove

ArchiveHeader::ArchiveHeader()
    : magic(),
      version( 1 ),
      archiveFlags( 0 ),
      numFileEntries( 0 ),
      fileEntryDataOffset( 0 )      // invalid offset on purpose!
{
    magic[0] = 0x89;
    magic[1] = 0x46;
    magic[2] = 0x41;
    magic[3] = 0x52;
    magic[4] = 0x0D;
    magic[5] = 0x0A;
    magic[6] = 0x1A;
    magic[7] = 0x0A;
}

ArchiveFileEntry::ArchiveFileEntry()
    : fileEntrySize( 0 ),
      uncompressedSize( 0 ),
      fileOffset( 0 ),
      checksum( 0 ),
      fileFlags( 0 )
{
    // change to stl algo
    memset( &filename[0], 0, MAX_FILENAME_LENGTH * sizeof(char) );
}

ArchiveFileEntry::ArchiveFileEntry( const std::string& filename_,
                                    const std::size_t archiveEntrySize,
                                    const std::size_t fileDataOffset,
                                    uint32_t checksum_ )
    : fileEntrySize( archiveEntrySize ),
      uncompressedSize( archiveEntrySize ),
      fileOffset( fileDataOffset ),
      checksum( checksum_ ),
      fileFlags( 0 ),
      filename()
{
    // Copy the name into a fixed length buffer, change to STL algo
    memset( &filename[0], 0, MAX_FILENAME_LENGTH * sizeof(char) );
    strncpy( &filename[0], filename_.c_str(), MAX_FILENAME_LENGTH * sizeof(char) );
}

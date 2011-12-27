#include <stdint.h>
#include <string>
#include <cstddef>

#include "thirdparty/sha2.h"
#include "constants.h"

/**
 * This is found at the beginning of every archive
 */
struct ArchiveHeader
{
    ArchiveHeader();

    uint8_t magic[8];       // file header 0x89 0x46 0x41 0x52 0D 0A 1A 0A
    uint8_t version;        // version the archive format
    uint8_t archiveFlags;   // 0: archive full compression
    uint32_t numFileEntries;// number of files in archive
    uint32_t fileEntryDataOffset; // XXX rename to fileEntryListOffset
    uint32_t archiveHash[SHA256_DIGEST_LENGTH]; // 32 bytes
} __attribute__((__packed__));

struct ArchiveFileEntry
{
    ArchiveFileEntry();
    ArchiveFileEntry( const std::string& filename_,
                      const std::size_t archiveEntrySize,
                      const std::size_t fileDataOffset,
                      uint32_t checksum_ );

    uint32_t fileEntrySize;     // Size of data as stored in archive
    uint32_t uncompressedSize;  // Size once uncompressed
    uint32_t fileOffset;        // Offset from start of file
    uint32_t checksum;          // CRC32 file signature
    uint8_t  fileFlags;         // 0: deleted, 1: compressed
    char     filename[MAX_FILENAME_LENGTH];
} __attribute__((__packed__));


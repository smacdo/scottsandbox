#ifndef SCOTT_ARCHIVE_FILEENTRY_H
#define SCOTT_ARCHIVE_FILEENTRY_H

#include <stdint.h>
#include <vector>
#include <string>

/**
 * Keeps tabs on an in memory file located inside of an archive
 */
class FileEntry
{
public:
    FileEntry( const std::string& filename_,
               uint8_t *pFileData_,
               size_t memorySize_ );

    std::string filename() const;
    std::size_t memorySize() const;
    const uint8_t * memoryPointer() const;
    void releaseMemory();

private:
    std::string mFilename;
    std::size_t mMemorySize;
    uint8_t * mpFileMemory;
};
#endif

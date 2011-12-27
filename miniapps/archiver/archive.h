#ifndef SCOTT_ARCHIVE_ARCHIVE_H
#define SCOTT_ARCHIVE_ARCHIVE_H

#include <stdint.h>
#include <vector>
#include <string>

#include "thirdparty/sha2.h"
#include "constants.h"
#include "archivedata.h"
#include "fileentry.h"

class Archive
{
public:
    Archive( const std::string& name );
    ~Archive();

    // Dumps information on the loaded archive
    void debugDump();

    // Open an archive file
    bool open( const std::string& filename );

    // Close an archive file
    void close();

    // Add a file to the archive
    bool add( const std::string& filename,
              const uint8_t* pData,
              std::size_t numberOfBytes );

    // Remove a file from the archive
    bool remove( const std::string& filename );

    // Check if a file is in the archive
    bool exists( const std::string& filename );

    // Gets the number of files in the archive
    size_t fileCount() const;

    // Get a listing of all the files in the archive
    std::vector<std::string> fileNameList() const;

    bool hasErrors() const;
    std::string errorMessage() const;
    size_t calculateFileDataStoreSize() const;
    bool save();

protected:
    void raiseError( const std::string& message );
    void clearErrors();
    bool validateHeader();

    bool load();
    void unload();

private:
    std::string mArchiveName;
    ArchiveHeader mHeader;
    std::vector<FileEntry> mFileEntries;
    std::string mErrorMessage;
};

#endif

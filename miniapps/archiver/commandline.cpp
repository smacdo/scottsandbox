/**
 * Copyright 2011 Scott MacDonald
 * archiver.cpp - Command line interaction with the file archive tool
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <fstream>
#include <stdint.h>
#include <vector>
#include <string>
#include <cassert>

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <iostream>
#include <boost/program_options.hpp>

#include "archive.h"

namespace po = boost::program_options;

bool executeHelp()
{
    std::cout << "help help" << std::endl;
    return true;
}

bool executeCreate( Archive& archive,
                    const std::string& target,
                    const std::vector<std::string>& input )
{
    std::cout << "CREATE: " << target << std::endl;
    bool actionStatus = true;

    // Now we should add files to the archive
    for ( size_t i = 0; i < input.size() && (!archive.hasErrors()); ++i )
    {
        std::string filename = input[i];

        // Open the requested file up and slurp it into an array of bytes
        std::ifstream ifs( filename.c_str(), std::ios::binary | std::ios::in );

        if (! ifs.good() )
        {
            std::cerr << "No such file: " << filename << std::endl;
            return false;
        }

        std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(ifs)),
                                    std::istreambuf_iterator<char>() );

        // Play nice and release the file
        ifs.close();

        // Print it to the console before adding it to the archive
        std::cout << "ADD   : " << filename << std::endl;

        archive.add( filename, &bytes[0], bytes.size() );
    }

    // Once the files have been added, and provided there were no errors,
    // commit the archive changes to disk
    if ( archive.hasErrors() == false )
    {
        archive.save();
    }
    else
    {
        std::cerr << "Encountered errors while adding files to archive. "
                  << "Not saving, sorry!"
                  << std::endl;
    }

    return actionStatus;
}

bool executeInfo( Archive& archive, const std::string& target )
{
    archive.open( target );
    archive.debugDump();

    return true;
}

bool executeList( Archive& archive, const std::string& target )
{
    archive.open( target );
    
    if (! archive.hasErrors() )
    {
        std::vector<std::string> files = archive.fileNameList();

        for ( size_t i = 0; i < files.size(); ++i )
        {
            std::cout << files[i] << std::endl;
        }
    }

    return true;
}

int main( int argc, char* argv[] )
{
    //
    // Figure out user action
    //
    po::options_description options("Program Options");
    options.add_options()
        ( "version,v",      "Print version string" )
        ( "help,h",         "Show help information" )
        ( "create,c",  po::value<std::string>(), "Create a new archive file with the name [name]" )
        ( "list,l",    po::value<std::string>(), "List all of the files in [archive]" )
        ( "add,a",     po::value<std::string>(), "Add one or more files to the archive" )
        ( "info,i",    po::value<std::string>(), "Show information about the archive" )
        ( "remove,r",  po::value<std::string>(), "Remove one or more files from the archive" )
        ( "extract,x", po::value<std::string>(), "Extract files from the archive" )
        ( "file,f", po::value< std::vector<std::string> >(), "Files to add/remove/update from archive" )
        ;

    po::positional_options_description p;
    p.add( "file", -1 );

    po::variables_map varmap;
    po::store( po::command_line_parser( argc, argv ).
                  options(options).positional(p).run(),
               varmap );
    po::notify( varmap );

    //
    // Get a list of files the user wanted to process
    //
    std::vector<std::string> filenames;
    
    if ( varmap.count("file") > 0 )
    {
        filenames = varmap["file"].as< std::vector<std::string> >();
    }

    //
    // Now perform user request
    //
    Archive target( "default.far" );
    bool didWork = false;

    if ( varmap.count("help") )
    {
        std::cout << options << std::endl;
        didWork = executeHelp();
    }
    else if ( varmap.count("create") )
    {
        std::string name = varmap["create"].as<std::string>();
        didWork = executeCreate( target, name, filenames );
    }
    else if ( varmap.count("list") )
    {
        std::string name = varmap["list"].as<std::string>();
        didWork = executeList( target, name );
    }
    else if ( varmap.count("info") )
    {
        std::string name = varmap["info"].as<std::string>();
        didWork = executeInfo( target, name );
    }
    else
    {
        std::cerr << "Unknown command, exiting" << std::endl;
    }

    //
    // See if the archive had errors
    // /
    if ( target.hasErrors() )
    {
        std::cerr << " ----- Error Report ----- " << std::endl
                  << target.errorMessage()        << std::endl
                  << std::endl;

        didWork = false;
    }

    return ( didWork ? EXIT_SUCCESS : EXIT_FAILURE );
}

/*
 * Copyright 2012 Scott MacDonald
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SCOTT_COMMON_APP_PLATFORM_H
#define SCOTT_COMMON_APP_PLATFORM_H

// Common includes
#include "common/macros.h"
#include "app/debug.h"

// System includes
#include <string>

/////////////////////////////////////////////////////////////////////////////
// Platform enumerations
/////////////////////////////////////////////////////////////////////////////
enum EProgramStatus
{
    EPROGRAM_OK = 0,
    EPROGRAM_ASSERT_FAILED = 2,
    EPROGRAM_FATAL_ERROR   = 5,
    EPROGRAM_USER_ERROR    = 6
};

/////////////////////////////////////////////////////////////////////////////
// Internal application utility functions
/////////////////////////////////////////////////////////////////////////////
namespace App
{
    enum EAssertionStatus
    {
        EAssertion_Halt = 0,
        EAssertion_Continue = 1,
        EAssertion_Default = 2
    };

    enum EErrorType
    {
        EERROR_WARNING,
        EERROR_ERROR,
        EERROR_FATAL
    };

    // Specifies the default handling of assertions.
    const EAssertionStatus GDefaultAssertionStatus = EAssertion_Halt;

    void setIsInUnitTestMode( bool isInUnitTesting );
    void setTestAssertsShouldDie( bool shouldBlowUp );
    void resetTestAssertsShouldDie();

    // Converts an error type enum into a string
    std::string getNameForError( EErrorType error );

    // Performs any needed platform specific work before starting the game
    void startup();
    void quit( EProgramStatus quitStatus, const std::string& message = "" );

    EAssertionStatus raiseAssertion( const char* message,
                                     const char* expression,
                                     const char* filename,
                                     unsigned int linenumber );

    void raiseError( const std::string& message,
                     const std::string& details = "" );

    void raiseFatalError( const std::string& message,
                          const std::string& details = "" );

    EAssertionStatus reportAssertion( const std::string& message,
                                      const std::string& expression,
                                      const std::string& filename,
                                      unsigned int line );

    void reportSoftwareError( const std::string& message,
                              const std::string& details,
                              EErrorType type,
                              unsigned int lineNumber,
                              const char * fileName,
                              const char * functionName );

    // Returns a "build string", which is a long string containing information
    // about the settings under which the game was built
    std::string getBuildString();
}

#endif

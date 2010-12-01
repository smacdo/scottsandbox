/**
 * List of application functions
 */

/**
 * Version information
 */
const int VersionMajor  = 0;
const int VersionMinor  = 1;
const int VersionPatch  = 1;

const int BuildNumber   = 1;

const char * RevisionId   = "(nil)";
const char * InternalName = "UnnamedProject";

/**
 * Build information
 */
const char * BuildDate = __DATE__;
const char * BuildTime = __TIME__;

/**
 * Build configuration
 */
#ifdef _DEBUG
const bool DebugRelease    = true;
#elseif
const bool DebugRelease    = false;
#endif

#ifdef __OPTIMIZE__
const bool OptimizeRelease = true;
#else
const bool OptimizeRelease = false;
#endif

/**
 * Returns a string representing information about the application's
 * build
 *
 * usually something like v2.0.0a REVISION_ID BUILD_ID (DATE;TIME)
 */
std::string getAppBuildString()
{
    std::stringstream ss;
    ss << "v" << getAppVersionString()
       << " " << RevisionId
       << " " << BuildNumber
       << "(" << BuildDate
       << ";" << BuildTime
       << ")";

    return ss.str();
}

std::string getAppBuildStringDetailed()
{
    std::stringstream ss;

    ss << "v" << getAppVersionString()
       << " " << RevisionId
       << " " << BuildNumber
       << "(" << BuildDate
       << ";" << BuildTime
       << ")" << std::endl
#ifdef __GNUC__
       << "GCC " << __VERSION__
#endif
       << std::endl;

    return ss.str();
}

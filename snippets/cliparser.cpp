
struct OptionInfo
{
    /**
     * Long name of the option
     */
    std::string name;

    /**
     * Uniquely identifies this option.
     */
    int         ident;

    /**
     * The short, one character name of this option
     */
    char        shortname;

    /**
     * A short descriptive text describing this option
     */
    std::string helptext;

    /**
     * Flag specifying if the option takes arguments
     */
    bool hasArguments;

    /**
     * Flag specifying if the option takes multiple arguments
     */
    bool hasManyArguments;

    /**
     * Flag specifying if the option is optional or required
     */
    bool optional;

    /**
     * Flag specifying if the option can be specified more than once
     */
    bool moreThanOnce;
};

struct Option
{
    int ident;
    std::vector<std::string> args;
};

struct CLParserResults
{
    /**
     * List of options that were parsed out of the command line
     */
    std::vector<Option> options;

    /**
     * List of extra parameters that were not parsed
     */
    std::vector<std::string> extra;
};

class CommandLineParser
{
public:

    void addOption( OptionInfo& opt );
    void addOptions( const std::vector<OptionInfo>& options );

    CLParserResults parse( size_t argc, const char** argv );
    CLParserResults parse( const std::vector<std::string>& argstr );

private:
    std::set<OptionInfo>  optionsinfo;
    std::multiset<Option> options;
};

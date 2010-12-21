#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <stack>
#include <sstream>

#include <stdlib.h>
#include <string.h>

// DEFINE_BUILTIN_SYMBOL( "nil", -1 )
// DEFINE_BUILTIN_SYMBOL( "false", 0 )
// DEFINE_BUILTIN_SYMBOL( "true", 1 )

enum TokenType
{
    TOK_OPEN,
    TOK_CLOSE,
    TOK_IDENT,
    TOK_NUMERIC,
    TOK_SEP,
    TOK_UNKNOWN,
    TOK_EOF
};

enum ValueType
{
    TYPE_NULL,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_UNKNOWN
};

struct Value
{
    Value()
        : iVal(0),
          type(TYPE_NULL)
    {
    }

    Value( int v )
        : iVal(v),
          type(TYPE_INT)
    {
    }

    Value( float v )
        : fVal(v),
          type(TYPE_FLOAT)
    {
    }

    Value( double v )
        : fVal( static_cast<float>(v) ),
          type(TYPE_FLOAT)
    {
    }

    Value( const std::string& str )
        : sVal( strdup( str.c_str() ) ),  // TODO THIS LEAKS
          type(TYPE_STRING)
    {
    }

    Value( bool b )
        : bVal( b ),
          type(TYPE_BOOL)
    {
    }

    std::string toString() const
    {
        std::stringstream ss;

        switch ( type )
        {
            case TYPE_NULL:
                ss << "(nil)";
                break;

            case TYPE_INT:
                ss << iVal;
                break;

            case TYPE_FLOAT:
                ss << fVal;
                break;

            case TYPE_BOOL:
                ss << ( bVal ? ":true" : ":false" );
                break;

            case TYPE_STRING:
                ss << sVal;
                break;
        }

        return ss.str();
    }

    bool operator == ( const Value& rhs )
    {
        if ( type != rhs.type ) { return false; }
        else if ( type == TYPE_NULL ) { return true; }
        else
        {
            if ( type == TYPE_INT )
            {
                return iVal == rhs.iVal;
            }
            else if ( type == TYPE_FLOAT )
            {
                return fVal == rhs.fVal;
            }
            else if ( type == TYPE_STRING )
            {
                return strcmp( sVal, rhs.sVal ) == 0;
            }
            else if ( type == TYPE_BOOL )
            {
                return bVal == rhs.bVal;
            }
            else
            {
                return false;
            }
        }
    }

    union
    {
        int iVal;
        float fVal;
        char * sVal;
        bool bVal;
    };
    ValueType type;
};

void printValue( const Value& val )
{
    switch ( val.type )
    {
        case TYPE_NULL:
            std::cout << "<type: null, value: (nil)>";
            break;
        case TYPE_INT:
            std::cout << "<type: int, value: " << val.iVal << ">";
            break;
        case TYPE_FLOAT:
            std::cout << "<type: float, value: " << val.fVal << ">";
            break;
        case TYPE_STRING:
            std::cout << "<type: string, value: " << val.sVal << ">";
            break;
        case TYPE_BOOL:
            std::cout << "<type: bool, value: "
                      << ( val.bVal ? ":true" : ":false" ) << ">";
            break;
        default:
            std::cout << "(unknown value type)";
    }
}

/**
 * An executing script
 */
class ScriptContext
{
public:

private:
};

/**
 * Base compiled expression class. All types of compiled expressions
 * derieve from here
 */
class Expression
{
    public:
        Expression()
            : params()
        {
        }

        Expression( const std::vector<Expression*> params )
            : params( params )
        {
        }

        virtual ~Expression()
        {
            for ( int i = 0; i < params.size(); ++i )
            {
                std::cout << "print " << i << std::endl;
                delete params[i];
            }
        }

        virtual std::string dump() const
        {
            return std::string("<BaseExpression>");
        }

        virtual Value evaluate()
        {
            return Value();
        }

        void addParam( Expression* param )
        {
            params.push_back( param );
        }

    protected:
        std::vector<Expression*> params;
};

/**
 * A function call expression. Basically if its not an value node or
 * an assignment node then it is a function call of some sort.
 */
class FuncCallExpression : public Expression
{
public:
    FuncCallExpression( const std::string& function,
                        const std::vector<Expression*> params )
        : Expression( params ),
          function( function )
    {
    }

    Value evaluate()
    {
        //
        // What kind of function is this?
        //  (horrible...)
        //
        if ( function == "-" )
        {
            // ASSUME
            return ( params[0]->evaluate().fVal - 
                     params[1]->evaluate().fVal );
        }
        if ( function == "+" )
        {
            return ( params[0]->evaluate().fVal +
                     params[1]->evaluate().fVal );
        }
        if ( function == "*" )
        {
            return ( params[0]->evaluate().fVal *
                     params[1]->evaluate().fVal );
        }
        if ( function == "test" )
        {
            Value v1 = params[0]->evaluate();
            Value v2 = params[1]->evaluate();
            bool  eq = v1 == v2;

            if ( eq )
            {
                // TODO move test to special function class impl
                std::cout << "Line X, PASSED: "
                          << v1.toString() << " = " << v2.toString()
                          << std::endl;
            }
            else
            {
                std::cout << "Line X, FAILED: "
                          << v1.toString() << " = " << v2.toString()
                          << std::endl;
            }

            return eq;
        }
        else
        {
            std::cerr << "<unknown function call: " << function
                      << ">" << std::endl;
            return Value();
        }
    }

    virtual std::string dump() const
    {
        return std::string("<function-call: ") + function +
               std::string(">");
    }

private:
    std::string function;
};

/**
 * A bare value expression - returns the value that it encapsulates
 */
class ValueExpression : public Expression
{
public:
    ValueExpression( const Value& value )
        : Expression(),
          value( value )
    {
    }

    Value evaluate()
    {
        return value;
    }

    virtual std::string dump() const
    {
        return std::string("<eval-const-value: ") + value.toString() +
               std::string(">");
    }

private:
    Value value;
};

std::string toString( TokenType t )
{
    switch ( t )
    {
        case TOK_OPEN:
            return "TOK_OPEN";
        case TOK_CLOSE:
            return "TOK_CLOSE";
        case TOK_IDENT:
            return "TOK_IDENT";
        case TOK_NUMERIC:
            return "TOK_NUMERIC";
        case TOK_SEP:
            return "TOK_SEP";
        case TOK_UNKNOWN:
            return "TOK_UNKNOWN";
        case TOK_EOF:
            return "TOK_EOF";
        default:
            return "TOK_NO_SUCH_TOK";
    }
}

bool isWhitespace( char c )
{
    return ( c == ' ' || c == '\t' || c == '\r' || c == '\n' );
}

bool isEndOfToken( char c )
{
    return ( isWhitespace(c) || c == '(' || c == ')' || c == ',' );
}

bool isNumeric( char c )
{
    return ( c >= '0' && c <= '9' );
}

bool isIdent( char c )
{
    return (c >= '*' && c <= 'z') || c == '%' || c == '&';
}

class Tokenizer
{
public:
    Tokenizer( const std::string str )
        : codestr( str ),
          lastTokenString(),
          lastTokenType( TOK_UNKNOWN ),
          currentPosition(0)
    {
    }

    ~Tokenizer()
    {
    }

    bool parseForNextToken()
    {
        int pos      = getCurrentPosition();
        assert( pos >= 0 && pos < codestr.size() );

        //
        // Consume whitespace
        //
        for ( pos; 
              pos < codestr.size() && isWhitespace(codestr[pos]);
            ++pos )
        {
            /* ... */
        }

        //
        // End of the line?
        //
        if ( pos == codestr.size() )
        {
            setLastToken( "", TOK_EOF );
            return false;
        }

        int startPos = pos;

        //
        // Search for the next token
        //
        TokenType type  = TOK_UNKNOWN;
        bool      done  = false;
        bool      error = false;

        for ( pos ;
            (!done) && pos < codestr.size() && (!isWhitespace(codestr[pos]))
                    && ((pos == startPos) || !isEndOfToken(codestr[pos]) );
            ++pos )
        {
            char c = codestr[pos];

            switch ( type )
            {
                case TOK_UNKNOWN:
                    if ( c == '(' )
                    {
                        type = TOK_OPEN;
                        done = true;
                    }
                    else if ( c == ')' )
                    {
                        type = TOK_CLOSE;
                        done = true;
                    }
                    else if ( c == ',' )
                    {
                        type = TOK_SEP;
                        done = true;
                    }
                    else if ( isNumeric( c ) )
                    {
                        type = TOK_NUMERIC;
                    }
                    else if ( isIdent( c ) )
                    {
                        type = TOK_IDENT;
                    }
                    else
                    {
                        error = true;
                    }
                    break;
                
                case TOK_NUMERIC:
                    if ( isNumeric( c ) == false )
                    {
                        error = true;
                    }
                    break;

                case TOK_IDENT:
                    if ( isIdent( c ) == false )
                    {
                        error = true;
                    }
                    break;

                default:
                    error = true;
            }

            //
            // Was there an error while parsing the token?
            //
            if ( error )
            {
                    std::cerr << "Error while parsing token. "
                            << "start=" << startPos << ", "
                            << "pos="   << pos      << ", "
                            << "type="  << toString(type) << ", "
                            << "value: "
                            << codestr.substr( startPos, pos - startPos+1 )
                            << std::endl;

                    return false;
            }
        }

        setLastToken( codestr.substr(startPos, pos - startPos), type );
        setCurrentPosition( pos );

        return !isEOF();
    }

    bool isEOF() const
    {
        return getCurrentPosition() >= codestr.size();
    }

    TokenType getLastTokenType() const
    {
        return lastTokenType;
    }

    std::string getLastToken() const
    {
        return lastTokenString;
    }

    private:
        void setLastToken( const std::string& str, TokenType type )
        {
            lastTokenString = str;
            lastTokenType   = type;
        }

        int getCurrentPosition() const { return currentPosition; }
        void setCurrentPosition( int p ) { currentPosition = p; }

        std::string codestr;
        std::string lastTokenString;
        TokenType   lastTokenType;
        int currentPosition;
};

class Compiler
{
public:
    Compiler( const std::string& codestr )
        : codestr( codestr )
    {
    }

    Expression* compile()
    {
        Tokenizer tokenizer( codestr );
        return compileStartTuple(tokenizer);
    }

    Expression* compileStartTuple( Tokenizer& tokenizer )
    {
        Expression * expr = NULL;
        std::vector<Expression*> params;
        std::stack<Expression*> stack;

        std::cout << "---" << std::endl;

        while ( tokenizer.parseForNextToken() )
        {
            std::cout << "@ "
                      << toString( tokenizer.getLastTokenType() ) << ": "
                      << tokenizer.getLastToken()
                      << std::endl;

            //
            // What is this element? If this is the start of an expression
            //
            if ( tokenizer.getLastTokenType() == TOK_OPEN )
            {
                std::cout << "^^^" << std::endl;
                params.push_back( compileStartTuple(tokenizer) );
                std::cout << "vvv" << std::endl;
            }
            else if ( tokenizer.getLastTokenType() == TOK_CLOSE )
            {
                break;
            }
            else if ( tokenizer.getLastTokenType() == TOK_IDENT )
            {
                params.push_back(
                        new ValueExpression( Value( tokenizer.getLastToken() ) )
                );
            }
            else if ( tokenizer.getLastTokenType() == TOK_NUMERIC )
            {
                params.push_back( 
                        new ValueExpression( 
                            Value( atof( tokenizer.getLastToken().c_str() ) )
                        )
                );
            }
        }

        expr = constructExpression( params );

        assert( expr != NULL );
        return expr;
    }

private:
    Expression* constructExpression( std::vector<Expression*> params ) const
    {
        //
        // Print it out
        //
        std::cout << "CONSTRUCT: ";

        for ( int i = 0; i < params.size(); ++i )
        {
            std::cout << params[i]->dump() << " ";
        }
    
        std::cout << " ~DONE~" << std::endl;

        //
        // Construct it
        //
        if ( params.size() == 1 )
        {
            return params[0];
        }
        else if ( params.size() > 2 )
        {
            return new FuncCallExpression(
                    params[0]->evaluate().toString(), 
                    std::vector<Expression*>( params.begin() + 1,
                                              params.end() )
            );
        }
        else
        {
            assert( false && "failed to construct" );
        }
    }

    std::string codestr;
};

int main( int argc, char* argv[] )
{
    std::string code = "(test 42 ( - 44 ( + 1 1 ) ) )  ";
    Compiler compiler(code);

    Expression* program = compiler.compile();
    
    printValue( program->evaluate() );

    std::cout << std::endl << " ---DONE--- " << std::endl;
}


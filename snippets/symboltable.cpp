#include <iostream>
#include <string>

typedef size_t Symbol;

class SymbolTable
{
    Symbol lookup( const std::string& name ) const;
    Symbol add( const std::string& name );
    bool   set( const std::string& name, Symbol s, bool errorOnExist=true );
    bool   contains( const std::string& name ) const;
    bool   contains( Symbol s ) const;
    bool   remove( const std::string& name );
    bool   remove( Symbol s );

    std::string getNameFor( Symbol s ) const;

    size_t count() const;
    Symbol nextUnusedSymbol() const;

    const static Symbol InvalidSymbol;

    typedef std::map<Symbol, std::string>                 SymbolMap;
    typedef std::map<Symbol, std::string>::iterator       SymbolMapItr;
    typedef std::map<Symbol, std::string>::const_iterator SymbolMapConstItr;
    typedef std::pair<Symbol, std::string>                SymbolNamePair;

private:
    SymbolMap symbols;
};

Symbol SymbolTable::InvalidSymbol = Symbol(0);

Symbol SymbolTable::lookup( const std::string& name ) const
{
    SymbolMapConstItr itr = symbols.find( name );

    if ( itr == symbols.end() )
    {
        return InvalidSymbol;
    }
    else
    {
        return itr.first;
    }
}

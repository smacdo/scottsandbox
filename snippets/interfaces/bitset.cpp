#include <iostream>
#include <googletest/googletest.h>

class BitSet
{
public:
    BitSet();
    BitSet( int bits );
    BitSet( const BitSet& bitset );

    BitSet& operator = ( const BitSet& rhs );

    bool operator == ( const BitSet& rhs ) const;
    bool operator != ( const BitSet& rhs ) const;
    

    BitSet( const BitSet& lhs );
    int cardinality() const;
    void clear();
    void clear( int index );
    void clear( int fromIndex, int toIndex );
    void flip( int index );
    void flip( int fromIndex, int toIndex );
    void get( int index ) const;
    BitSet slice( int fromIndex, int toIndex ) const;
    bool intersects( BitSet set ) const;
    bool isEmpty() const;
    int numBits() const;
    int nextClearBit( int fromIndex ) const;
    int nextSetBit( int fromIndex ) const;

    BitSet and( const BitSet& rhs ) const;
    BitSet or( const BitSet& rhs ) const;
    
    void set( const index );
    void set( const index, bool value );
void 
};

int main( int argc , char* argv[] )
{
    testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}

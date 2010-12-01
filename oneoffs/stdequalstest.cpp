#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

template<typename T>
inline bool pred( const T& a, const T& b )
{
    return a == b;
}

/*
bool pred( int a, int b )
{
    return a == b; 
}
*/

int main() 
{
    vector<int> v;
    v.push_back( 2 );
    v.push_back( 0 );
    v.push_back( 5 );

    vector<int> a( v );

    if ( std::equal( v.begin(), v.end(), a.begin(), pred<int> ) )
    {
        std::cout << "works" << std::endl;
    }
    else
    {
        std::cout << "borked" << std::endl;
    }
}

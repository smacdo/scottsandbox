/**
 * Provides a typesafe way for the safe dereferencing of pointers,
 * and as a way of discouraging excessively deep nesting of data structures.
 *
 * Examples:
 *   int val = 48;
 *   int *v0 = &value;      // dangerous, each dereference of v0 must check
 *   int &v1 = deref( v0 )
 */
#ifndef SCOTT_COMMON_DEREF_H
#define SCOTT_COMMON_DEREF_H

template<typename T>
inline static T& derefT( T* ptr,
                         const char* var,
                         const char* file,
                         unsigned int line )
{
    if ( ptr == NULL )
    {
        Platform::RaiseFatalError("System", file, line)
            << "Attempted to dereference a null pointer '" << var << "'";
        Platform::Exit();
    }

    return *ptr;
}

template<typename T>
inline static const T& derefT( const T* ptr,
                               const char* var,
                               const char* file,
                               unsigned int line )
{
    if ( ptr == NULL )
    {
        Platform::RaiseFatalError("System", file, line)
            << "Attempted to dereference a null pointer '" << var << "'";
        Platform::Exit();
    }

    return *ptr;
}

#ifdef NDEBUG
#   define deref(x) (*(x))
#else
#   define deref(x) derefT(x, #x, __FILE__, __LINE__)
#endif

#endif

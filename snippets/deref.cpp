// Copyright (c) 2010 Scott MacDonald.
// Code is public domain

/////////////////////////////////////////////////////////////////////////////
// HEADER
/////////////////////////////////////////////////////////////////////////////
// rev 2
#ifndef SCOTT_WORKBENCH_NULLDEREF_H
#define SCOTT_WORKBENCH_NULLDEREF_H

/**
 * Demonstration of the "deref" template function. This function
 * allows you to safely cast a pointer to a reference using a named
 * macro-like method.
 *
 * eg, simple case using it would be:
 *  Foo * ptr = ...
 *  Foo & ref = deref(ptr)
 *
 * However usually you would use this to create references in your code
 * and then safely cast pointers.
 *  deref(getActivePlayer()).doSomething()
 *
 */
#include <cassert>

template<typename T>
inline T& safe_deref( T * ptr )
{
    assert( ptr != 0 && "Pointer must not be null" );
    return *ptr;
}

template<typename T>
inline T& deref( T * ptr )
{
#if (defined(DEBUG) || !defined(_DEBUG)) && !defined(DISABLE_DEREF_CHECK)
    assert( ptr != 0 && "Pointer must not be null" );
#endif
    return *ptr;
}

#endif

/////////////////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////////////////
#include <googletest/googletest.h>

struct Foo
{
    int val;
};

TEST(Deref,DerefIntWithValue)
{
    int original = 42;
    int *     p  = &original;

    EXPECT_EQ( 42, deref(p) );
    EXPECT_EQ( 42, safe_deref(p) );
}

TEST(Deref,DerefStructureDataMember)
{
    Foo structure;
    Foo * p = &structure;

    p->val       = 0;        // old way
    deref(p).val = 42;       // safe way

    EXPECT_EQ( 42, structure.val );
}

TEST(Deref,DerefConstIntReference)
{
    int  original = 42;
    int& result   = deref( &original );

    EXPECT_EQ( 42, result );
}

TEST(Deref,DerefConstPointer)
{
    int  original = 42;
    const int * p = &original;

    const int& result   = deref( p );
    EXPECT_EQ( 42, result );
}

TEST(Deref,DerefAssertsOnNull)
{
    // This should work even with the above _DEBUG conditional, because
    // unit tests should always be built with debugging enabled
    int * p = NULL;
    EXPECT_DEATH( deref(p), "Pointer must not be null" );
}

TEST(Deref,SafeDerefAssertsOnNull)
{
    int * p = NULL;
    EXPECT_DEATH( safe_deref(p), "Pointer must not be null" );
}

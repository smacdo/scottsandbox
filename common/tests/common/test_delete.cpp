/**
 * Unit tests for the memory deleter. Do NOT THREAD
 */
#include <googletest/googletest.h>
#include <common/delete.h>
#include <ostream>
#include <vector>

#include "common/delete.h"

class TestDummy
{
public:
    TestDummy( size_t& totalCounter, size_t& selfCounter )
        : mTotalCounter( totalCounter ),
          mSelfCounter( selfCounter )
    {
        mTotalCounter++;
        mSelfCounter++;
    }

    ~TestDummy()
    {
        mTotalCounter--;
        mSelfCounter--;
    }

private:
    size_t& mTotalCounter;
    size_t& mSelfCounter;
};

TEST(UtilsTests,InternalTest_VerifyTestDummy)
{
    size_t total = 0;
    size_t a     = 0;
    size_t b     = 0;

    TestDummy *pA = new TestDummy( total, a );
    EXPECT_EQ( 1u, total );
    EXPECT_EQ( 1u, a );
    EXPECT_EQ( 0u, b );

    TestDummy *pB = new TestDummy( total, b );
    EXPECT_EQ( 2u, total );
    EXPECT_EQ( 1u, a );
    EXPECT_EQ( 1u, b );

    delete pA;
    EXPECT_EQ( 1u, total );
    EXPECT_EQ( 0u, a );
    EXPECT_EQ( 1u, b );

    delete pB;
    EXPECT_EQ( 0u, total );
    EXPECT_EQ( 0u, a );
    EXPECT_EQ( 0u, b );

}

TEST(UtilsTests,DeletePointer)
{
    size_t total = 0;
    size_t a = 0;

    TestDummy *pA = new TestDummy( total, a );
    Delete( pA );

    EXPECT_EQ( 0u, total );
    EXPECT_EQ( 0u, a );

    EXPECT_TRUE( pA == NULL );
}

TEST(UtilsTest,DeletePointerArray)
{
    size_t total = 0;
    size_t a     = 0;

    // TODO: Implement
    EXPECT_TRUE( true );
}

TEST(UtilsTests,DeletePointerGenericContainerWhichIsAVector)
{
    size_t total = 0;
    size_t a     = 0;
    size_t b     = 0;
    size_t c     = 0;

    std::vector<TestDummy*> v;
    v.push_back( new TestDummy( total, a ) );
    v.push_back( new TestDummy( total, b ) );
    v.push_back( new TestDummy( total, c ) );

    EXPECT_EQ( 3u, total );
    EXPECT_EQ( 1u, a );
    EXPECT_EQ( 1u, b );
    EXPECT_EQ( 1u, c );

    EXPECT_EQ( 3u, DeletePointerContainer( v ) );
    EXPECT_EQ( 0u, total );
    EXPECT_EQ( 0u, a );
    EXPECT_EQ( 0u, b );
    EXPECT_EQ( 0u, c );
}

TEST(UtilsTests,DeleteVectorPointers)
{
    size_t total = 0;
    size_t a     = 0;
    size_t b     = 0;
    size_t c     = 0;

    std::vector<TestDummy*> v;
    v.push_back( new TestDummy( total, a ) );
    v.push_back( new TestDummy( total, b ) );
    v.push_back( new TestDummy( total, c ) );

    EXPECT_EQ( 3u, total );
    EXPECT_EQ( 1u, a );
    EXPECT_EQ( 1u, b );
    EXPECT_EQ( 1u, c );

    EXPECT_EQ( 3u, DeleteVectorPointers( v ) );
    EXPECT_EQ( 0u, total );
    EXPECT_EQ( 0u, a );
    EXPECT_EQ( 0u, b );
    EXPECT_EQ( 0u, c );
}

TEST(UtilsTests,DeleteMapPointers)
{
    size_t total = 0;
    size_t a     = 0;
    size_t b     = 0;
    size_t c     = 0;

    std::map<int, TestDummy*> v;
    v.insert( std::pair<int, TestDummy*>( 0, new TestDummy( total, a ) ) );
    v.insert( std::pair<int, TestDummy*>( 1, new TestDummy( total, b ) ) );
    v.insert( std::pair<int, TestDummy*>( 2, new TestDummy( total, c ) ) );

    EXPECT_EQ( 3u, total );
    EXPECT_EQ( 1u, a );
    EXPECT_EQ( 1u, b );
    EXPECT_EQ( 1u, c );

    EXPECT_EQ( 3u, DeleteMapPointers( v ) );
    EXPECT_EQ( 0u, total );
    EXPECT_EQ( 0u, a );
    EXPECT_EQ( 0u, b );
    EXPECT_EQ( 0u, c );
}



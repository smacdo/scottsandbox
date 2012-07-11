#include <googletest/googletest.h>
#include <common/assert.h>

class AssertTests : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        Debug::setTestAssertShouldDie( true );
    }

    virtual void TearDown()
    {
        Debug::resetTestAssertShouldDie();
    }
};

typedef AssertTests AssertDeathTests;

TEST_F(AssertTests,AssertSuccess)
{
    ASSERT( 1 == 1 );
}

TEST_F(AssertDeathTests,AssertFailed)
{
    EXPECT_DEATH( { ASSERT( 1 == 0 ); }, "ASSERTION FAILED: 1 == 0" );
}

TEST_F(AssertTests,AssertNullSuccess)
{
    int *p = NULL;
    ASSERT_NULL( p );
}

TEST_F(AssertTests,AssertNotNullSuccess)
{
    int *p = new int;
    ASSERT_NOT_NULLl( p );

    delete p;
}

TEST_F(AssertTests,AssertCore)
{
    CORE_ASSERT( true, "This is a libcommon assertion" );
}

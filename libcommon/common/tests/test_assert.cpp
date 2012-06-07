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
    assert( 1 == 1 );
}

TEST_F(AssertDeathTests,AssertFailed)
{
    EXPECT_DEATH( { assert( 1 == 0 ); }, "ASSERTION FAILED: 1 == 0" );
}

TEST_F(AssertTests,AssertNullSuccess)
{
    int *p = NULL;
    assert_null( p );
}

TEST_F(AssertTests,AssertNotNullSuccess)
{
    int *p = new int;
    assert_notNull( p );

    delete p;
}

TEST_F(AssertTests,AssertCore)
{
    core_assert( true, "This is a libcommon assertion" );
}

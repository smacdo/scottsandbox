#include <googletest/googletest.h>
#include <testing/testing.h>
#include <common/assert.h>

class AssertTests : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
//        Tests::setTestAssertShouldDie( true );
    }

    virtual void TearDown()
    {
//        Tests::resetTestAssertShouldDie();
    }
};

typedef AssertTests AssertDeathTests;

TEST_F(AssertTests,AssertSuccess)
{
    ASSERT( 1 == 1 );
}

TEST_F(AssertTests,AssertSuccess_MessageAssertMacro)
{
    ASSERT_MSG( 1 == 1, "HELLO THERE" );
}

TEST_F(AssertDeathTests,AssertFailed)
{
    EXPECT_DEATH( { ASSERT( 1 == 0 ); }, "ASSERTION FAILED: 1 == 0" );
}

TEST_F(AssertDeathTests,AssertFailed_MessageMacro)
{
    EXPECT_DEATH( { ASSERT_MSG( 1 == 0, "HAI" ); }, "ASSERTION FAILED: 1 == 0" );
}

TEST_F(AssertTests,AssertNullSuccess)
{
    int *p = NULL;
    ASSERT_NULL( p );
}

TEST_F(AssertTests,AssertNotNullSuccess)
{
    int *p = new int;
    ASSERT_NOT_NULL( p );

    delete p;
}

TEST_F(AssertTests,VerifySuccess)
{
    VERIFY( 2 == 2 );
}

TEST_F(AssertDeathTests,VerifyFailure)
{
    EXPECT_DEATH( { VERIFY( 5 == 1 ); }, "ASSERTION FAILED: 5 == 1" );
}

TEST_F(AssertTests,AssertCore)
{
    CORE_ASSERT( true, "This is a libcommon assertion" );
}

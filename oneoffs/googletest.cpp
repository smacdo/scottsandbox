/**
 * Samples of google test code
 */
#include <iostream>
#include <gtest/gtest.h>
#include <cstring>

//  ::testing::AssertionFailure() << #expr << "Generic assertion failed"; \
//
bool GTestIsRunning = true;
#define ENABLE_ASSERTIONS 1
#define ASSERT_USE_GTEST  1

struct UnitTestAssertionEx
{
    UnitTestAssertionEx( const char * file,
                         const char * func,
                         const char * expr,
                         int line )
        : file(file), func(func), expr(expr), line(line)
    {
    }

    bool operator == ( const char* rhs_expr ) const
    {
        return (strcmp( expr, rhs_expr ) == 0);
    }

    bool operator != ( const char* rhs_expr ) const
    {
        return (strcmp( expr, rhs_expr ) != 0);
    }

    const char * file;
    const char * func;
    const char * expr;
    int line;
};

#ifdef ENABLE_ASSERTIONS
#ifdef assert
#undef assert
#endif

#ifdef ASSERT_USE_GTEST
#define assert(x) EXPECT_TRUE(x) << "Source: application internal assertion macro"
#else
#define assert(x) raiseAssertion(#x)
#endif
#endif

#define custom_assert(expr,message) \
        if ( (expr) == false ) \
        { \
            if ( GTestIsRunning ) \
            { \
                throw UnitTestAssertionEx(__FILE__,__func__,#expr,__LINE__); \
            } \
            else \
            { \
                raiseAssertion(#expr); \
            } \
        }

#define my_assert(name,x)  if ( (x) == false ) \
                           { \
                            SCOPED_TRACE(#name); \
                            raiseAssertion(#x); \
                           }

#define expect_assert(expr_msg,block) \
    try \
    { \
        block; \
        ADD_FAILURE() \
         << "Did not trigger expected assertion message '" << expr_msg \
         << "'"; \
    } \
    catch( UnitTestAssertionEx& uta ) \
    { \
        if ( uta != expr_msg ) \
        { \
            std::cerr << "An unexpected assertion was thrown while running " \
                      << "the current unit test. It will be aborted. "       \
                      << std::endl                                           \
                      << "\tFILE: " << uta.file << std::endl                 \
                      << "\tFUNC: " << uta.func << std::endl                 \
                      << "\tLINE: " << uta.line << std::endl                 \
                      << "\tEXPR: " << uta.expr << std::endl;                \
        } \
    } \
    catch ( ... ) \
    { \
        std::cerr << "Exception thrown, but was not of UnitTestAssertionEx " \
                  << "type!" << std::endl;                                   \
    }

void raiseAssertion(const char*);

int crashable( int f )
{
    custom_assert(f <  3,"Param must be less than two");
//  assert( f < 2 );
    std::cout << "value is: " << f << std::endl;
    return 42;
}

void raiseAssertion( const char * expr )
{
    std::cout << "ASSERTION FAILED: " << expr << std::endl;
    exit(1);
}
int main( int argc, char* argv[] )
{
    int results = 1;

    try
    {
        testing::InitGoogleTest(&argc, argv);
        results = RUN_ALL_TESTS();
    }
    catch ( UnitTestAssertionEx& ex )
    {
        std::cerr << "An uncaught assertion was thrown while running unit "
                  << "tests"
                  << std::endl;
        results = -42;
    }
    catch ( ... )
    {
        std::cerr << "An uncaught exception was thrown while running unit "
                  << "tests. Aborting"
                  << std::endl;
        results = -44;
    }

    return results;
}

TEST(Tests,RaiseAnyAssertion)
{
    expect_assert("f < 5", crashable(4));
    expect_assert("f < 5", crashable(1));
}

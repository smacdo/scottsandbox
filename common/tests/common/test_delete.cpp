/**
 * Unit tests for the memory deleter. Do NOT THREAD
 */
#include <googletest/googletest.h>
#include <common/delete.h>

class Widget
{
public:
    Widget( size_t& counter )
        : m_counter(counter)
    {
        m_counter++;
    }
    
    ~Widget()
    {
        m_counter--;
    }

private:
    Widget( const Widget& );
    Widget& operator = ( const Widget& );

    size_t& m_counter;
};

class CommonTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        m_objCounter = 0;
    }

    Widget* create()
    {
        return new Widget( m_objCounter );
    }

    size_t objCount() const
    {
        return m_objCounter;
    }

private:
    size_t m_objCounter;
};

TEST_F(CommonTest,DeleteCheckedWorks)
{
    Widget* w = create();
    EXPECT_EQ( 1, objCount() );

    CheckedDelete( w );

    ASSERT_EQ( 0, objCount() );
}

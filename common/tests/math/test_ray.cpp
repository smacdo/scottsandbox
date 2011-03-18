/**
 * Unit tests for ray class
 */
#include <common/math/geoms.h>
#include <googletest/googletest.h>

TEST(Geoms,EmptyRay)
{
    Ray a;
    Vec3 z( 0.0, 0.0, 0.0 );

    EXPECT_EQ( z, a.origin()   );
    EXPECT_EQ( z, a.direction());
}

TEST(Geoms,SimpleRay)
{
    Ray r( Vec3(1,1,1), Vec3(1,0,0) );

    EXPECT_EQ( Vec3(1,1,1), r.origin()    );
    EXPECT_EQ( Vec3(1,0,0), r.direction() );
}

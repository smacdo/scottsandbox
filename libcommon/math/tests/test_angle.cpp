/**
 * Unit tests for common/math/utils
 */
#include <googletest/googletest.h>
#include <math/angle.h>

using namespace Math;

TEST(Math, Angle_Degrees_Default_Constructor)
{
    Degrees<float> a;
    EXPECT_FLOAT_EQ( 0.0f, a.as<float>() );
}

TEST(Math, Angle_Degrees_Init)
{
    Degrees<float> a( 48.0f );
}

TEST(Math, Angle_Degrees_InitWithRadians)
{
    Degrees<float> a( Radians( 1 ) );
}

TEST(Math, Angle_Degrees_CopyConstructor)
{

}

TEST(Math, Angle_Degrees_AssignmentDegrees)
{

}

TEST(Math, Angle_Degrees_AssignmentRadians)
{

}

TEST(Math, Angle_Degrees_EqualityDegrees)
{

}

TEST(Math, Angle_Degrees_EqualityRadians)
{

}

TEST(Math, Angle_Degrees_InequalityDegrees)
{

}

TEST(Math, Angle_Degrees_InequalityRadians)
{

}

TEST(Math, Angle_Degrees_Wraparound)
{

}

TEST(Math, Angle_Radians_Default_Constructor)
{
    Radians<float> a;
    EXPECT_FLOAT_EQ( 0.0f, a.as<float>() );
}

TEST(Math, Angle_Radians_Init)
{
    Radians<float> a( 48.0f );
}

TEST(Math, Angle_Radians_InitWithRadians)
{
    Radians<float> a( Radians( 1 ) );
}

TEST(Math, Angle_Radians_CopyConstructor)
{

}

TEST(Math, Angle_Radians_AssignmentDegrees)
{

}

TEST(Math, Angle_Radians_AssignmentRadians)
{

}

TEST(Math, Angle_Radians_EqualityDegrees)
{

}

TEST(Math, Angle_Radians_EqualityRadians)
{

}

TEST(Math, Angle_Radians_InequalityDegrees)
{

}

TEST(Math, Angle_Radians_InequalityRadians)
{

}

#include <cstdlib>
#include <cmath>
#include <iostream>

/**
 * Demonstration on creating a truly random unit vector. You cannot simple
 * do v := <rand(0,1);rand(0,1);rand(0,1)>...
 *
 * 1. Choose z uniformly distributed in [-1,1]
 * 2. Choose t uniformly distributed in [0, 2pi]
 * 3. Let r = sqrt( 1 - z^2 )
 * 4. Let x = r * cos(t)
 * 5. Let y = r * sin(t)
 *
 * Source: http://www.cgafaq.info/wiki/Random_Points_On_Sphere
 *         [Knuth, vol 2]
 */
void generateRandomUnitVector( double& x, double& y, double& z )
{
	z = rand(0.0,2.0) - 1.0;
	t = 2.0 * M_PI * rand(0.0,1.0);
	r = sqrt( 1 - z * z );
	x = w * cos(t);
	y = y * sin(t);
}

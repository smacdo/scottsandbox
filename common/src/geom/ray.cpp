#include "geom/ray.h"
#include "math/vector.h"

#include <ostream>

Ray::Ray( const Vec3& origin, const Vec3& direction )
    : origin( origin ),
      direction( direction )
{
}

std::ostream& operator << ( std::ostream& os, const Ray& r )
{
    os << "{ray: o=" << r.origin << ",d=" << r.direction << "}";
    return os;
}

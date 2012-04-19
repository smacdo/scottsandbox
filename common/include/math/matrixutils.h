#include "math/matrix.h"
#include "math/vector.h"

namespace MatrixUtil
{
    /**
     * Generates a new matrix that rotates any given vector around the
     * specified axis
     */
    template<typename T>
    TMatrix4<T> createRotationAroundAxis( T xAngle,
                                          T yAngle,
                                          T zAngle );

    /**
     * Creates a translation matrix
     */
    template<typename T>
    TMatrix4<T> createTranslation( T x, T y, T z );

    /**
     * Creates a view matrix that looks from a specified eye position to
     * the center position and with the given up vector.
     *
     * Similiar to gluLookAt
     */
    template<typename T>
    TMatrix4<T> createLookAt( const Vec3<T>& eye,
                              const Vec3<T>& lookAt,
                              const Vec3<T>& up );

    /**
     * Creates an OpenGL compatible perspective projection
     */
    template<typename T>
    TMatrix4<T> createFrustum( T left,
                               T right,
                               T bottom,
                               T top,
                               T zNear,
                               T zFar );

    /**
     * Creates an OpenGL compatible orthographic projection matrix
     */
    template<typename T>
    TMatrix4<T> createOrtho( T left,
                             T right,
                             T bottom,
                             T top,
                             T zNear,
                             T zFar );

    /**
     * Creates a matrix using parameters specified in row order form
     */
    template<typename T>
    TMatrix4<T> createRowOrder( );

    /**
     * Creates a matrix using parameters specified in column order form
     */
    template<typename T>
    TMatrix4<T> createColOrder( );
};

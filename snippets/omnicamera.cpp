
class OmniCamera
{
public:
    OmniCamera();

    /**
     * Adds rotation factor to camera
     */
    void addRotation( float roll, float pitch );

    /**
     * Resets camera's rotation to the given roll and pitch
     */
    void setRotation( float roll, float pitch );

    /**
     * Sets the camera's movement speed
     */
    void setMovementSpeed( float accel=1.0f, float max=10.0f );

    /**
     * Camera movement speed
     */
    float movementSpeed() const;

    /**
     * Move camera along the given directional vector, mainting the current
     * looking direction
     */
    void move( const Vec3& axis );

    /**
     * Moves the camera forward
     */
    void moveForward();

    /**
     * Moves the camera backward
     */
    void moveBackward();

    /**
     * Moves the camera to the right (strafe right)
     */
    void moveRight();

    /**
     * Moves the camra to the left (strafe left)
     */
    void moveLeft();
    
    /**
     * Positions the camera at the given coordinates
     */
    void placeAt( const Vec3& axis );

    /**
     * Calculates and returns the camera's viewing frustum
     */
    Frustum cameraFrustum() const;

    /**
     * Returns the camera's 4x4 perspective transformation matrix
     */
    Mat4 perspectiveTransform() const;

    /**
     * Returns the camera's ortho 4x4 transformation matrix
     */
    Mat4 orthoTransform() const;

    float nearViewDistance() const { return m_nearViewDistance; }
    float farViewDistance() const  { return m_farViewDistance;  }
    float fieldOfView() const      { return m_fieldOfView;      }
    float aspectRatio() cont       { return m_aspectRatio;      }
    Vec3  position() const         { return m_position;         }
    Vec3  rotation() const         { return m_rotation;         }
    Vec3  up()       const         { return m_up;               }
    Vec3  right()    const         { return m_right;            }
    Vec3  lookAtPoint() const      { ... }
protected:
    /**
     * Given a new camera position and new rotation vector, method will
     * recalculate internal cached transform information
     */
    void update( const Vec3& newPosition, const Vec3& newRotation );

private:
    /**
     * Flag marking if the camera's values have changed and the transform
     * matrix needs to be recalculated
     */
    bool m_dirty;

    /**
     * Near viewing distance of camera
     */
    float m_nearViewDistance;

    /**
     * Far viewing distance of camera
     */
    float m_farViewDistance;

    /**
     * Field of view for camera
     */
    float m_fieldOfView;

    /**
     * Aspect ratio
     */
    float m_aspectRatio;

    /**
     * XYZ position of camera, relative to world origin
     */
    Vec3 m_position;

    /**
     * Roll/pitch/yaw rotation vector
     */
    Vec3 m_rotation;
    
    /**
     * Vector representing the camera's up vector
     */
    Vec3 m_up;

    /**
     * Vector representing the camera's right side vector
     */
    Vec3 m_right;

    /**
     * 4x4 transformation matrix, calculated from the position and up
     * matrix
     */
    mutable Mat4 m_transform;
};

OmniCamera::OmniCamera()
    : m_dirty( false ),
      m_nearViewDistance( 0.1 ),
      m_farViewDistance( 1000.0 ),
      m_fieldOfView( 75.0 ),
      m_aspectRatio( 0.0 ),
      m_position( 0.0, 0.0, 10.0 ),
      m_rotation( 0.0, 0.0, 0.0 ),
      m_up( 0.0, 1.0, 0.0 ),
      m_right( 1.0, 0.0, 0.0 ),
      m_transform()
{
}


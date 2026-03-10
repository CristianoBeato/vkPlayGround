
#ifndef __FRUSTRUM_HPP__
#define __FRUSTRUM_HPP__

class crFrustrum
{
public:
    enum planes_e
    {
        PLANE_TOP,
        PLANE_BOTTOM,
        PLANE_RIGHT,
        PLANE_LEFT,
        PLANE_FRONT, // also know as far
        PLANE_BACK, // also know as near
        NUM_PLANES,
    };

    struct Plane_s
    {
        crVec3f normal;
        float   distance;

        // Returns the signed distance from a point to the plane.
        inline float GetSignedDistanceToPlane( const crVec3f& in_point ) const 
        {
            return normal.Dot( in_point ) - distance;
        }
    };

    crFrustrum( void );
    ~crFrustrum( void );

    void Update( const crVec3f &in_viewPos, const float in_aspect, const float in_fovY, const float in_zNear, const float in_zFar );
    void Update( const crMatrix4& in_viewProj );
    bool IsBoxInside( const crBoundsVolume &in_bounds );
    bool IsSphereInside( const crBoundsSphere &in_sphere ) const;

private:
    Plane_s m_faces[NUM_PLANES];
};


#endif //!__FRUSTRUM_HPP__
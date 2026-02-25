
#ifndef __BOUNDS_HPP__
#define __BOUNDS_HPP__

class crBounds
{
public:
    crBounds( void );
    ~crBounds( void );

private:
    crVec3f     m_min;
    crVec3f     m_max;
};

#endif //!__BOUNDS_HPP__
#ifndef RAYTRACER_RAY_HEADER
#define RAYTRACER_RAY_HEADER

#include "core\point.h"
#include "core\vector.h"

namespace rt {

class Ray {
public:
    Point o;
    Vector d;
	bool m_bMousePicking;
	int m_annotationID;
	

    Ray() {}
    Ray(const Point& o, const Vector& d): o(o), d(d), m_bMousePicking(false), m_annotationID(-1) {}

    Point getPoint(float distance) const;
};

}

#endif

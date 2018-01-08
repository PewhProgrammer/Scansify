#ifndef RAYTRACER_RAY_HEADER
#define RAYTRACER_RAY_HEADER

#include "core\point.h"
#include "core\vector.h"

namespace rt {

class Ray {
public:
    Point o;
    Vector d;

    Ray() {}
    Ray(const Point& o, const Vector& d): o(o), d(d) {}

    Point getPoint(float distance) const;
};

}

#endif

#ifndef RAYTRACER_SOLIDS_INFINITEPLANE_HEADER
#define RAYTRACER_SOLIDS_INFINITEPLANE_HEADER

#include "../core/float4.h"
#include "../core/vector.h"

#include "../bbox.h"
#include "../intersection.h"

namespace rt {

class InfinitePlane{
private:
	Point origin;
	Vector normal;
public:
    InfinitePlane() {}
    InfinitePlane(const Point& origin, const Vector& normal):
		origin(origin),normal(normal){}

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
	virtual Point sample() const;
    virtual float getArea() const;
};

}


#endif

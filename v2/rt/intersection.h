#ifndef CG1RAYTRACER_INTERSECTION_HEADER
#define CG1RAYTRACER_INTERSECTION_HEADER

#include "ray.h"
#include "core/vector.h"
#include "core/scalar.h"

namespace rt {

class Solid;

class Intersection {
private:
	Intersection(float distance) :distance(distance){}
	Point uv;
public:
    Ray ray;
    double distance;
	Vector normal;

    Intersection() {}
    static Intersection failure();
	Intersection(double distance, const Ray& ray, Vector normal, const Point& uv) :distance(distance), ray(ray), normal(normal), uv(uv) {};

    Point hitPoint() const;

    Point local() const;

    operator bool(); //this allows intersection object to be put directly in conditional statements. Becomes true iff there is an intersection
};

}

#endif

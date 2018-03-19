#ifndef CG1RAYTRACER_INTERSECTION_HEADER
#define CG1RAYTRACER_INTERSECTION_HEADER

#include "ray.h"
#include "core/vector.h"
#include "core/scalar.h"

namespace rt {

class Solid;
class SmoothTriangle;

class Intersection {
private:
	Intersection(float distance) :distance(distance){}
	Point uv;
public:
    Ray ray;
    double distance;
	Vector normal;
	SmoothTriangle* solid;
	bool m_bShowAnnotation = false;
	int m_nodeCounter;
	int m_annotationID;

    Intersection() {}
    static Intersection failure();
	Intersection(double distance, const SmoothTriangle* solid,  const Ray& ray, Vector normal, const Point& uv) :distance(distance), ray(ray), normal(normal), uv(uv), m_nodeCounter(-1), m_annotationID(-1){};

    Point hitPoint() const;
    Point local() const;

    operator bool(); //this allows intersection object to be put directly in conditional statements. Becomes true iff there is an intersection
};

}

#endif

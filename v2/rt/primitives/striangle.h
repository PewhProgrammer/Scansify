#ifndef CG1RAYTRACER_SOLIDS_STRIANGLE_HEADER
#define CG1RAYTRACER_SOLIDS_STRIANGLE_HEADER


#include "../intersection.h"
#include "../bbox.h"


namespace rt {

class SmoothTriangle {
private:
	Vector normals[3];
	Point A, B, C;
	Vector ab, bc, ac;
	float invTriArea;
	float TriArea;
	Point boxmin, boxmax;
public:
    SmoothTriangle() {}
    SmoothTriangle(Point vertices[3], Vector normals[3]);
    SmoothTriangle(const Point& v1, const Point& v2, const Point& v3, 
        const Vector& n1, const Vector& n2, const Vector& n3);

	virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
	virtual Point sample() const;
	virtual float getArea() const;
};

}

#endif

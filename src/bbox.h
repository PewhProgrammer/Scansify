#ifndef RAYTRACER_BBOX_HEADER
#define RAYTRACER_BBOX_HEADER

#include <utility>
#include "core/point.h"
#include "core/vector.h"

#include <map>
#include "ray.h"

namespace rt {

class Ray;

class BBox {
public:
    Point min, max;

    BBox() {}
    BBox(const Point& min, const Point& max) {
		this->min = min;
		this->max = max;
	}
    static BBox empty();
    static BBox full();

    void extend(const Point& point);
    void extend(const BBox& bbox);

	bool contains(rt::Point p);

    Vector diagonal() const { return max - min; }

    std::pair<float,float> intersect(const Ray& ray) const;

    bool isUnbound();
	bool BBox::isEmpty();
};

}


#endif

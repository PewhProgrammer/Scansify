#ifndef RAYTRACER_CAMERAS_PERSPECTIVE_HEADER
#define RAYTRACER_CAMERAS_PERSPECTIVE_HEADER


#include "../../core/vector.h"
#include "../../core/point.h"

#include "../../ray.h"

namespace rt {

class PerspectiveCamera {

private:
	Point center;
	Vector forward;
	Vector spanX, spanY; 


public:
	PerspectiveCamera(
		const Point& center,
		const Vector& forward,
		const Vector& up,
		float verticalOpeningAngle,
		float horizonalOpeningAngle
	);

    virtual Ray getPrimaryRay(float x, float y) const;
};

}


#endif

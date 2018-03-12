#ifndef RAYTRACER_CAMERAS_PERSPECTIVE_HEADER
#define RAYTRACER_CAMERAS_PERSPECTIVE_HEADER


#include "../core/vector.h"
#include "../core/point.h"

#include "../ray.h"

namespace rt {

class PerspectiveCamera {

private:
	Vector spanX, spanY; 


public:
	Point center;
	Vector forward;
	Vector up;
	PerspectiveCamera();
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

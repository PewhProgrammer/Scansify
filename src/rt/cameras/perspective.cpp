#include "perspective.h"

//image plane computation for coord x,y
#define ratio(x,y) tan(x*0.5f) / tan(y*0.5f);

/*
Easy computation of spanX/spanY Vector by using ratio of Opposite and Adjacent 
*/

namespace rt {
	PerspectiveCamera::PerspectiveCamera(const Point & center, const Vector & forward, const Vector & up, float verticalOpeningAngle, float horizonalOpeningAngle)
	{

		this->center = center;  
		this->forward = forward.normalize();

		spanX = cross(this->forward, up).normalize() * tan(horizonalOpeningAngle * 0.5f);
		spanY = cross(this->forward, spanX).normalize() * tan(verticalOpeningAngle * 0.5f);

	}
	Ray PerspectiveCamera::getPrimaryRay(float x, float y) const
	{
		Vector vec = forward + x * spanX + y*spanY;
		vec = vec.normalize();
		return Ray(center, vec.normalize());
	}
}
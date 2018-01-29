#include "intersection.h"

namespace rt {
	Intersection Intersection::failure()
	{
		return Intersection(-1);
	}

	Point Intersection::hitPoint() const
	{
		return this->ray.o + this->ray.d * (float)distance; 
	}

	Point Intersection::local() const
	{
		return uv;
	}
	Intersection::operator bool()
	{
		return distance > 0; //will treat special as true case
	}
}
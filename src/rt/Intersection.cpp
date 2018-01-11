#include "intersection.h"

namespace rt {
	Intersection Intersection::failure()
	{
		return Intersection(-1);
	}
	Intersection::Intersection(float distance, const Ray & ray, const Point & uv)
	{
		this->distance = distance; 
		this->ray = ray;  
		this->uv = uv; 
	}
	Point Intersection::hitPoint() const
	{
		return this->ray.o + this->ray.d * distance; 
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
#include "infiniteplane.h"

namespace rt {
	BBox InfinitePlane::getBounds() const
	{
		return BBox(Point().rep(-FLT_MAX),Point().rep(FLT_MAX));
	}
	Intersection InfinitePlane::intersect(const Ray & ray, float previousBestDistance) const
	{
		/*
		Intersection computation:
		ray r = o + td
		plane equation = (p_1 - (o + td)) * n = 0 
		<-> 
		- n(o+td) =  p_1 * n
		<->
		t = (p_1 * n + n* o) / - n* d
		*/
		

		double omega = -dot(ray.o - origin, normal);
		double delta = dot(ray.d, normal);

		//Wenn die Ray.Origin innerhalb der Plane liegt
		double check = dot(ray.o - origin, normal);
		if (check == 0) {
			return Intersection::failure();
		}

		if (dot(origin - ray.o, normal) == 0)
			return Intersection::failure();
		if (delta == 0)
			return Intersection::failure();
		else {
			double t = omega / delta;
			
			if (t > previousBestDistance || t < 0)
				return Intersection::failure();
			return Intersection(t, ray,  normal, ray.o + (float)t*ray.d);
		}


		/*float dividend = dot(normal, (origin - ray.o)); 
		float divisor = dot(normal, ray.d); 

		if (dividend == 0) return Intersection::failure();

		float t = dividend / divisor;
		if (t > previousBestDistance || t < 0) return Intersection::failure();
		
		Point uvw =  origin - Point(0.0f, 0.0f, 0.0f) + (ray.o + t*ray.d);
		return Intersection(t, ray, this, normal,uvw);*/

	}
	Point InfinitePlane::sample() const
	{
		NOT_IMPLEMENTED; 
	}
	float InfinitePlane::getArea() const
	{
		return FLT_MAX;
	}
}
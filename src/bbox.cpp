#include "bbox.h"

typedef std::pair<float, float> f2;

namespace rt {
	BBox BBox::empty()
	{
		return BBox(Point().rep(FLT_MAX), Point().rep(-FLT_MAX));
	}
	BBox BBox::full()
	{
		return BBox(Point().rep(-FLT_MAX), Point().rep(FLT_MAX)) ;
	}
	void BBox::extend(const Point & point)
	{
		min = rt::min(point, min);
		max = rt::max(point, max);

	}
	void BBox::extend(const BBox & bbox)
	{
		max = rt::max(bbox.max, max);
		min = rt::min(bbox.min, min);

	}
	std::pair<float, float> BBox::intersect(const rt::Ray & ray) const
	{
	
		//if unbound return intersection immediately?
		

		float invRayDir, near, far;
		float t_0 = -FLT_MAX; // needs to be overwritten later 
		float t_1 = FLT_MAX;
		for (int i = 0; i < 3; i++) {
			if (ray.d[i] != 0)
				invRayDir = 1.f / ray.d[i]; // more efficient
			else
				continue;
			near = (min[i] - ray.o[i]) * invRayDir;
			far = (max[i] - ray.o[i]) * invRayDir;
			if (near > far) std::swap(near, far); // assign near and far

			//biggest near and smallest far for slabs
			t_0 = near > t_0 ? near : t_0;
			t_1 = far < t_1 ? far : t_1; 
		}

		//if biggest near is smaller than smallest t1, we have intersection
		//if not then t0 > t1 and we throw failure
		return f2(t_0, t_1);
	}

	bool BBox::isUnbound()
	{
		return
			(min.x == -FLT_MAX) |
			( min.y == -FLT_MAX) |
			( min.z == -FLT_MAX) |
			(max.x == FLT_MAX ) |
			(max.y == FLT_MAX) |
			(max.z == FLT_MAX);
	}

	bool BBox::isEmpty()
	{
		return
			(min.x == FLT_MAX) |
			(min.y == FLT_MAX) |
			(min.z == FLT_MAX) |
			(max.x == FLT_MIN) |
			(max.y == FLT_MIN) |
			(max.z == FLT_MIN);
	}
}
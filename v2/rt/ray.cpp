#include "ray.h"

namespace rt {

	Point Ray::getPoint(float distance) const
	{
		return this->o + (d * distance);
	}
}
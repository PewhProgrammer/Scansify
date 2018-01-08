#include "point.h"

namespace rt {


	Vector Point::operator-(const Point & b) const
	{
		return Vector(this->x - b.x , this->y - b.y , this->z - b.z);
	}

	bool Point::operator==(const Point & b) const
	{
		return this->x == b.x && this->y == b.y && this->z == b.z; 
	}

	bool Point::operator!=(const Point & b) const
	{
		return !(*this == b); 
	}

	Point operator*(float scalar, const Point & b)
	{
		return Point(b.x * scalar , b.y * scalar , b.z * scalar);
	}

	Point operator*(const Point & a, float scalar)
	{
		return scalar * a; 
	}

	Point min(const Point & a, const Point & b)
	{

		return Point(
			a.x < b.x ? a.x : b.x,
			a.y < b.y ? a.y : b.y,
			a.z < b.z ? a.z : b.z);
	}

	Point max(const Point & a, const Point & b)
	{
		return Point(
			a.x > b.x ? a.x : b.x,
			a.y > b.y ? a.y : b.y,
			a.z > b.z ? a.z : b.z);
	}

	float Point::operator[](int idx) const
	{
		return p[idx];
	}

	Point Point::assignValue(int idx, float ivalue) const
	{
		Point r = Point(p[0],p[1],p[2]); 
		r.p[idx] = ivalue; 
		return r;
	}

	Point::Point(const Float4 & f4)
	{
		float x = f4[0] / f4[3]; 
		float y = f4[1] / f4[3];
		float z = f4[2] / f4[3];

		this->x = x;
		this->y = y;
		this->z = z;

		p[0] = x;
		p[1] = y;
		p[2] = z;
	}

	Point::Point(float x, float y, float z)
	{
		this->x = x; this->y = y; this->z = z;
		p[0] = x; p[1] = y; p[2] = z;
	}

}


#include "vector.h" 

namespace rt {

		Vector Vector::normalize() const
		{
			return *this / this->length();
		}

		Vector Vector::operator+(const Vector & b) const
		{
			return  Vector(this->x + b.x, this->y + b.y, this->z + b.z);
		}


		Vector Vector::operator-(const Vector & b) const
		{
			return Vector(this->x - b.x , this->y - b.y , this->z - b.z);
		}

		Vector Vector::operator-() const
		{
			return Vector(-this->x, -this->y, -this->z);
		}


		float Vector::lensqr() const
		{
			return sqr(this->x) + sqr(this->y) + sqr(this->z);
		}

		float Vector::length() const
		{
			return sqrt(this->lensqr()) ;
		}

		bool Vector::operator==(const Vector & b) const
		{
			return this->x == b.x && this->y == b.y && this->z == b.z; 
		}

		bool Vector::operator!=(const Vector & b) const
		{
			return !(*this == b);
		}


		Vector operator*(float scalar, const Vector & b)
		{
			return Vector(b.x * scalar , b.y * scalar , b.z * scalar);
		}

		Vector operator*(const Vector & a, float scalar)
		{
			return scalar * a;
		}

		Vector operator/(const Vector & a, float scalar)
		{
			if (scalar == 0.f) {
				return a;
			}
			return Vector(a.x / scalar , a.y / scalar , a.z / scalar);
		}

		Vector cross(const Vector & a, const Vector & b)
		{
			return Vector(
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x)
				;
		}

		float dot(const Vector & a, const Vector & b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		Vector min(const Vector & a, const Vector & b)
		{

			return Vector(
				a.x < b.x ? a.x : b.x,
				a.y < b.y ? a.y : b.y,
				a.z < b.z ? a.z : b.z);
		}

		Vector max(const Vector & a, const Vector & b)
		{
			return Vector(
				a.x > b.x ? a.x : b.x,
				a.y > b.y ? a.y : b.y,
				a.z > b.z ? a.z : b.z);
		}

		Point operator+(const Point & a, const Vector & b)
		{
			return Point(a.x + b.x , a.y + b.y , a.z + b.z );
		}

		Point operator+(const Vector & a, const Point & b)
		{
			return b + a; 
		}

		Point operator-(const Point & a, const Vector & b)
		{
			return Point(a.x - b.x, a.y - b.y, a.z - b.z);
		}

		Point operator*(const Float4 & scale, const Point & p)
		{
			return Point();
		}

		float Vector::operator[](int idx) const
		{
			return v[idx];
		}

		Vector::Vector(const Float4 & f4)
		{
			this->x = f4[0];
			this->y = f4[1];
			this->z = f4[2];

			v[0] = f4[0]; v[1] = f4[1]; v[2] = f4[2];
		}

		Vector::Vector(float x, float y, float z)
		{
			this->x = x; 
			this->y = y;
			this->z = z;

			v[0] = x; v[1] = y; v[2] = z;
		}

}



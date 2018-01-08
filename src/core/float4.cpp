#include "float4.h"

namespace rt {

	Float4::Float4(float x, float y, float z, float w) {
		f4[0] = x;
		f4[1] = y;
		f4[2] = z;
		f4[3] = w;
	}

	Float4::Float4(const Point& p) {
		f4[0] = p.x;
		f4[1] = p.y;
		f4[2] = p.z;
		f4[3] = 1.0f;
	}
	Float4::Float4(const Vector& v) {
		f4[0] = v.x;
		f4[1] = v.y;
		f4[2] = v.z;
		f4[3] = 0.f;
	}

	float& Float4::operator[](int idx) {
		return f4[idx];
	}

	float Float4::operator[](int idx) const {
		return f4[idx];
	}

	Float4 Float4::operator+(const Float4& b) const {
		return Float4(f4[0] + b.f4[0], f4[1] + b.f4[1], f4[2] + b.f4[2], f4[3] + b.f4[3]);
	}
	Float4 Float4::operator-(const Float4& b) const {
		return Float4(f4[0] - b.f4[0], f4[1] - b.f4[1], f4[2] - b.f4[2], f4[3] - b.f4[3]);
	}
	Float4 Float4::operator*(const Float4& b) const {
		return Float4(f4[0] * b.f4[0], f4[1] * b.f4[1], f4[2] * b.f4[2], f4[3] * b.f4[3]);
	}
	Float4 Float4::operator/(const Float4& b) const {
		return Float4(f4[0] / b.f4[0], f4[1] / b.f4[1], f4[2] / b.f4[2], f4[3] / b.f4[3]);
	}


	Float4 Float4::operator-() const {
		return Float4(-f4[0], -f4[1], -f4[2], -f4[3]);
	}

	bool Float4::operator==(const Float4& b) const {
		return f4[0] == b.f4[0] & f4[1] == b.f4[1] & f4[2] == b.f4[2] & f4[3] == b.f4[3];
	}
	bool Float4::operator!=(const Float4& b) const {
		return !(this->operator==(b));
	}


	Float4 operator*(float scalar, const Float4& b) {
		return Float4().rep(scalar) * b;
	}
	Float4 operator*(const Float4& a, float scalar) {
		return Float4(a[0] * scalar, a[1] * scalar, a[2] * scalar, a[3] * scalar);
	}
	Float4 operator/(const Float4& a, float scalar) {
		assert(scalar != 0) << "div 0 scalar";
		return Float4(a[0] / scalar, a[1] / scalar, a[2] / scalar, a[3] / scalar);
	}
	float dot(const Float4& a, const Float4& b) {
		float result= 0; 
		for (int i = 0; i < 4; i++) {
			result += a[i] * b[i];
		}

		return result;
	}

	Float4 min_(const Float4& a, const Float4& b) {
		return Float4(
					min(a[0], b[0]), 
					min(a[1],b[1]),
					min(a[2],b[2]),
					min(a[3],b[3]));
	}
	Float4 max_(const Float4& a, const Float4& b) {
		return Float4(
			max(a[0], b[0]),
			max(a[1], b[1]),
			max(a[2], b[2]),
			max(a[3], b[3]));
	}
}
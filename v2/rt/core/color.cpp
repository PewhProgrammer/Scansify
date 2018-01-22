#include "color.h"

namespace rt {


	RGBColor RGBColor::operator+(const RGBColor& c) const {
		return RGBColor(this->r + c.r, this->g + c.g, this->b + c.b);
	}

	RGBColor RGBColor::operator-(const RGBColor& c) const {
		return RGBColor(this->r - c.r, this->g - c.g, this->b - c.b);
	}

	RGBColor RGBColor::operator*(const RGBColor& c) const {
		return RGBColor(this->r * c.r, this->g * c.g, this->b * c.b);
	}

	bool RGBColor::operator==(const RGBColor& b) const {
		return this->r == b.r && this->g == b.g && this->b == b.b;
	}
	bool RGBColor::operator!=(const RGBColor& b) const {
		return !(*this == b) ;
	}

	RGBColor RGBColor::clamp() const {
		float r = this->r > 1 ? 1 : this->r;
		float g = this->g > 1 ? 1 : this->g;
		float b = this->b > 1 ? 1 : this->b;

		r = this->r < 0 ? 0 : r;
		g = this->g < 0 ? 0 : g;
		b = this->b < 0 ? 0 : b;
		
		return RGBColor(r, g, b);
	}

	RGBColor RGBColor::gamma(float gam) const {
		NOT_IMPLEMENTED;
	}

	float RGBColor::luminance() const {
		NOT_IMPLEMENTED;
	}

	RGBColor operator*(float scalar, const RGBColor& b) {
		return RGBColor(b.r * scalar, b.g * scalar, b.b * scalar);
	}

	RGBColor operator*(const RGBColor& a, float scalar) {
		return scalar * a; 
	}

	RGBColor operator/(const RGBColor& a, float scalar) {
		return RGBColor(a.r / scalar, a.g / scalar, a.b / scalar);
	}
}
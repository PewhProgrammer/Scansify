#ifndef RAYTRACER_POINT_HEADER
#define RAYTRACER_POINT_HEADER

#include "macros.h"
#include "vector.h"
#include "float4.h"

namespace rt {

class Vector;
class Float4;

class ALIGN(16) Point {
private:
	float p[3];
public:
    float x, y, z;

    Point() {}
	Point(float x, float y, float z);
	float operator[](int idx) const;
	Point assignValue(int idx,float ivalue) const;

    explicit Point(const Float4& f4);

    static Point rep(float v) { return Point(v,v,v); }

    Vector operator-(const Point& b) const;

    bool operator==(const Point& b) const;
    bool operator!=(const Point& b) const;

};

Point operator*(float scalar, const Point& b);
Point operator*(const Point& a, float scalar);

Point min(const Point& a, const Point& b);
Point max(const Point& a, const Point& b);

}

#endif

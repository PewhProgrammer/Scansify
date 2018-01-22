#include "interpolate.h"
#include "assert.h"

namespace rt {

    template <typename T>
    T lerp(const T& px0, const T& px1, float xPoint) {
		//return px0 + xPoint*(px1 - px0); // assuming xPoint is in [0,1]
		return (1 - xPoint) * px0 + xPoint * px1;
    }

    template <typename T>
    T lerpbar(const T& a, const T& b, const T& c, float aWeight, float bWeight) {
		//apply barycentric coordinate
		float cWeight = 1 - aWeight - bWeight;

		return a*aWeight + b*bWeight + c*cWeight;
    }

    template <typename T>
    T lerp2d(const T& px0y0, const T& px1y0, const T& px0y1, const T& px1y1, float xWeight, float yWeight) {

		T x_axis_y0 = lerp(px0y0, px1y0, xWeight);
		T x_axis_y1 = lerp(px0y1, px1y1, xWeight);

		T y_axis = lerp(x_axis_y0, x_axis_y1, yWeight);

		return y_axis;
    }

    template <typename T>
    T lerp3d(const T& px0y0z0, const T& px1y0z0, const T& px0y1z0, const T& px1y1z0,
        const T& px0y0z1, const T& px1y0z1, const T& px0y1z1, const T& px1y1z1,
        float xPoint, float yPoint, float zPoint) {
			
		T front = lerp2d(px0y0z0, px1y0z0, px0y1z0, px1y1z0, xPoint, yPoint);
		T back = lerp2d(px0y0z1, px1y0z1, px0y1z1, px1y1z1, xPoint, yPoint);

		T inter_BackFront = lerp(front, back, zPoint);
		return inter_BackFront;
    }

}


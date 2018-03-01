#include "striangle.h"

#include "infiniteplane.h"
#include "../core/interpolate.h"
#include "../core/random.h"

namespace rt {

	rt::SmoothTriangle::SmoothTriangle(Point vertices[3], Vector normals[3])
	{
		A = vertices[0];
		B = vertices[1];
		C = vertices[2];

		ab = B - A;
		bc = C - B;
		ac = C - A;

		this->normals[0] = normals[0].normalize();
		this->normals[1] = normals[1].normalize();
		this->normals[2] = normals[2].normalize();

		this->planeNormal = cross(ab, ac).normalize();

		this->TriArea = cross(ab, ac).length() * 0.5f;
		this->invTriArea = 1 / TriArea;

		boxmin = min(min(vertices[0], vertices[1]), vertices[2]);
		boxmax = max(max(vertices[0], vertices[1]), vertices[2]);

		// TODO change this to false 
		m_bAnnotated = true;
	}

	rt::SmoothTriangle::SmoothTriangle(const Point & v1, const Point & v2, const Point & v3, const Vector & n1, const Vector & n2, const Vector & n3)
	{
		Point p[3];
		Vector v[3];

		p[0] = v1; p[1] = v2; p[2] = v3;
		v[0] = n1; v[1] = n2; v[2] = n3;

		SmoothTriangle(p, v);
	}

	BBox SmoothTriangle::getBounds() const
	{
		return BBox(boxmin, boxmax);
	}

	Intersection rt::SmoothTriangle::intersect(const Ray & ray, float previousBestDistance) const
	{
		InfinitePlane plane = InfinitePlane(A, planeNormal);
		Intersection hit = plane.intersect(ray, previousBestDistance);
		if (!hit || hit.distance > previousBestDistance) return Intersection::failure();

		Point P = hit.hitPoint();

		float ABP_area = cross(ab, P - A).length() * 0.5f;
		float ACP_area = cross(ac, P - A).length() * 0.5f;
		float BCP_area = cross(bc, P - B).length() * 0.5f;

		float u, v, w;

		u = ABP_area * invTriArea;
		v = ACP_area * invTriArea;
		w = BCP_area * invTriArea;

		double sum = u + v + w;

		if (sum > 1.0001)
			return Intersection::failure();

		Vector normal = u*normals[2] + v*normals[1] + w*normals[0];
		//Vector normal = lerpbar(normals[2], normals[1], normals[0], u, v); //essentially the same
		const SmoothTriangle* k = this;

		return Intersection(hit.distance, this, ray, normal, Point(u, v, w));
	}

	Point SmoothTriangle::sample() const
	{

		float rand1 = random();
		float rand2 = random();

		if ((rand1 + rand2)  < 1) {
			return (lerpbar(A, B, C, rand1, rand2));
		}
		else {
			return (lerpbar(A, B, C, 1 - rand1, 1 - rand2));
		}
	}
	float SmoothTriangle::getArea() const
	{
		return this->TriArea;
	}
}
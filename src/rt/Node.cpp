#include "Node.h"

using namespace rt;

typedef std::pair<float, float> f2;

rt::Node::Node()
{
	this->boundingBox = BBox::empty();
}

bool rt::Node::isLeaf()
{
	if ( objects.size() > 0 || this->right==nullptr || this->left == nullptr || this->Leaf )
		return true;
	return false; 
}

Intersection rt::Node::searchIntersection(const Ray& r,float previousDistance)
{
	if (isLeaf()) {
		/*Intersection mainBox = Intersection::failure(); 
		for (auto &primitive : objects) {
			Intersection hit = primitive->intersect(r, previousDistance);
			if (hit) {
				previousDistance = hit.distance;
				mainBox = hit;
			}
		}
		return mainBox; */
		f2 result = this->boundingBox.intersect(r);
		if (result.first > result.second) return Intersection::failure();

		return Intersection(result.first,r,rt::Point(0,0,0));
	}

	f2 LBoundHit = left->boundingBox.intersect(r);
	f2 RBoundHit = right->boundingBox.intersect(r);
	Intersection LHit = Intersection::failure(), RHit = Intersection::failure();

	if (LBoundHit.first <= LBoundHit.second) LHit = left->searchIntersection(r,previousDistance);
	if (LHit) previousDistance = LHit.distance;

	if (RBoundHit.first <= RBoundHit.second) RHit = right->searchIntersection(r, previousDistance);
	if (RHit) return RHit; 

	return LHit;
}

void rt::Node::add(Point prim)
{
	objects.push_back(prim);
	boundingBox.extend(prim);
}

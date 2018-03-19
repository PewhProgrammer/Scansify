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
		Intersection mainBox = Intersection::failure(); 
		for (auto &primitive : objects) {
			Intersection hit = primitive->intersect(r, previousDistance);
			if (hit) {
				hit.solid = primitive;
				previousDistance = hit.distance;
				mainBox = hit;
				if (r.m_bMousePicking) {
					this->m_bAnnotated = true;
					mainBox.m_nodeCounter = 0;
					mainBox.m_annotationID = r.m_annotationID;
				}
			}
		}
		return mainBox;
	}

	f2 LBoundHit = left->boundingBox.intersect(r);
	f2 RBoundHit = right->boundingBox.intersect(r);
	Intersection LHit = Intersection::failure(), RHit = Intersection::failure();

	if (LBoundHit.first <= LBoundHit.second) LHit = left->searchIntersection(r,previousDistance);
	if (LHit) { 
		if (LHit.m_nodeCounter == 0) {
			this->m_bAnnotated = true; 
			this->m_annotationID = r.m_annotationID;
		}
		LHit.m_nodeCounter--;

		previousDistance = (float)LHit.distance;
	}

	if (RBoundHit.first <= RBoundHit.second) RHit = right->searchIntersection(r, previousDistance);
	if (RHit) { 
		if (RHit.m_nodeCounter == 0) {
			this->m_bAnnotated = true;
			this->m_annotationID = r.m_annotationID;
		}
		RHit.m_nodeCounter--;

		if (this->m_bAnnotated) {
			RHit.m_annotationID = this->m_annotationID;
			RHit.m_bShowAnnotation = true;
		}
		return RHit; 
	}

	if (this->m_bAnnotated) {
		LHit.m_annotationID = this->m_annotationID;
		LHit.m_bShowAnnotation = true;
	}
	return LHit;
}

void rt::Node::add(SmoothTriangle* prim)
{
	objects.push_back(prim);
	boundingBox.extend(prim->getBounds());
}

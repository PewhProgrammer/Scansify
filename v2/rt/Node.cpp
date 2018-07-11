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

Intersection rt::Node::searchIntersection(const Ray& r, float previousDistance)
{
	std::vector<Node*> k;
	return searchIntersection(r, previousDistance, k);
}

Intersection rt::Node::searchIntersection(const Ray& r,float previousDistance, std::vector<Node*>& annotated)
{
	if (isLeaf()) {
		Intersection resultingHit = Intersection::failure(); 
		for (auto &primitive : objects) {
			Intersection hit = primitive->intersect(r, previousDistance);
			if (hit) {
				resultingHit = hit;
				resultingHit.solid = primitive;

				if (this->m_bAnnotated) {
					std::map<int, Point> toAnnotate;
					for (auto it = m_annotationMap.begin(); it != m_annotationMap.end(); it++) {
						auto len = (it->second - hit.hitPoint()).length();
						if (len <= 0.0012f) {
							resultingHit.m_bShowAnnotation = true;
							toAnnotate[it->first] = it->second;
						}
						//printf("Error measurement %s for: %.5f\n", (len <= 0.0012f) ? "succeded" : "failed",  len);
					}

					resultingHit.m_IDtoPoints = toAnnotate;
				}

				if (r.m_bMousePicking) {

					this->m_bAnnotated = true;
					resultingHit.m_nodeCounter = 0;

					m_annotationMap[r.m_annotationID] = hit.hitPoint(); // add hitpoint to map
					annotated.push_back(this);
				}
				previousDistance = hit.distance;
			}
		}
		return resultingHit;
	}

	f2 LBoundHit = left->boundingBox.intersect(r);
	f2 RBoundHit = right->boundingBox.intersect(r);
	Intersection LHit = Intersection::failure(), RHit = Intersection::failure();

	if (LBoundHit.first <= LBoundHit.second) LHit = left->searchIntersection(r,previousDistance, annotated);
	if (LHit) { 
		if (LHit.m_nodeCounter == 0) {
			this->m_bAnnotated = true; 
			annotated.push_back(this);
		}
		LHit.m_nodeCounter--;

		previousDistance = (float)LHit.distance;
	}

	if (RBoundHit.first <= RBoundHit.second) RHit = right->searchIntersection(r, previousDistance, annotated);
	if (RHit) { 
		if (RHit.m_nodeCounter == 0) {
			this->m_bAnnotated = true;
			annotated.push_back(this);
		}
		 RHit.m_nodeCounter--;

		return RHit; 
	}


	return LHit;
}

void rt::Node::add(SmoothTriangle* prim)
{
	objects.push_back(prim);
	boundingBox.extend(prim->getBounds());
}

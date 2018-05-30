#ifndef RAYTRACER_GROUPS_BVH_HEADER
#define RAYTRACER_GROUPS_BVH_HEADER

#include "Node.h"
#include "bbox.h"
#include "core/point.h"
#include "intersection.h"
#include "primitives\striangle.h"

namespace rt {

class BVH  {
private:

	int NodeNum = 0;

	std::vector<SmoothTriangle*> SceneObjects;
	Node* Root;
	uint16_t depth;
	uint8_t c_trav = 1;
	uint8_t c_isect = 80;

	virtual void buildTree(Node* node);
	virtual void clearAnnotation(Node* node);
	virtual void splitPane(Node* node);
	virtual std::pair<float, float> splitInTheMiddle(Node* node);
	virtual std::pair<float, float> SAH(Node* node);
public:
	bool built_flag = false;

    BVH();
    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX, std::vector<Node*>& a = std::vector<Node*>()) const;
    virtual void buildIndex();
	virtual void rebuildIndex();
	virtual ~BVH();
    virtual void add(SmoothTriangle* s);
};

}

#endif

#ifndef RAYTRACER_GROUPS_BVH_HEADER
#define RAYTRACER_GROUPS_BVH_HEADER

#include "Node.h"
#include "../bbox.h"
#include "../core/point.h"
#include "intersection.h"

namespace rt {

class BVH  {
private:

	int NodeNum = 0;

	std::vector<Point> SceneObjects;
	Node* Root;
	bool built_flag = false; 
	uint16_t depth;
	uint8_t c_trav = 1;
	uint8_t c_isect = 80;

	virtual void buildTree(Node* node);
	virtual void splitPane(Node* node);
	virtual std::pair<float, float> splitInTheMiddle(Node* node);
	virtual std::pair<float, float> SAH(Node* node);
public:
    BVH();
    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
    virtual void rebuildIndex();
	virtual ~BVH();
    virtual void add(Point p);
};

}

#endif

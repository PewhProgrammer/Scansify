#pragma once

#include <vector>
#include "../bbox.h"
#include <stdio.h>
#include "intersection.h"

namespace rt{

class Node  {
public:

	int id;
	std::vector<Point> objects;

	BBox boundingBox = BBox::empty();

	Node* right;
	Node* left;
	bool Leaf = false; 

	Node();
	bool isLeaf();

	Intersection searchIntersection(const Ray& r,float previousDistance);
	void add(Point prim);
};

}
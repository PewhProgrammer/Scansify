#pragma once

#include <vector>
#include<set>
#include "bbox.h"
#include <stdio.h>
#include "intersection.h"
#include "primitives\striangle.h"

namespace rt{

class Node  {
public:

	int id;
	std::vector<SmoothTriangle*> objects;

	BBox boundingBox = BBox::empty();

	Node* right;
	Node* left;
	bool Leaf = false; 
	std::set<int> m_annotationID;

	/// <summary>
	/// flag to show annotated areas upon rendering process
	/// </summary>
	bool m_bAnnotated = false;

	Node();
	bool isLeaf();

	Intersection searchIntersection(const Ray& r, float previousDistance);
	Intersection searchIntersection(const Ray& r,float previousDistance, std::vector<Node*>& annotated);
	void add(SmoothTriangle* prim);
};

}
#pragma once

#include <vector>
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
	int m_annotationID = -1;

	/// <summary>
	/// flag to show annotated areas upon rendering process
	/// </summary>
	bool m_bAnnotated = false;

	Node();
	bool isLeaf();

	Intersection searchIntersection(const Ray& r,float previousDistance);
	void add(SmoothTriangle* prim);
};

}
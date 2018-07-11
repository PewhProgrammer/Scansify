#pragma once

#include <vector>
#include "bbox.h"
#include <stdio.h>
#include "intersection.h"
#include "primitives\striangle.h"
#include <map>

namespace rt{

class Node  {
public:

	int id;
	bool Leaf = false;

	std::vector<SmoothTriangle*> objects;
	BBox boundingBox = BBox::empty();
	Node* right;
	Node* left;


	/// <summary>
	/// flag to show annotated areas upon rendering process
	/// </summary>
	bool m_bAnnotated = false;

	/// <summary>
	/// To see coherent annotations
	/// </summary>
	std::map<int, Point> m_annotationMap;

	/// <summary>
	/// outputs this hitpoint when already hit before
	/// </summary>
	Point m_annotatedHit;



	Node();
	bool isLeaf();

	Intersection searchIntersection(const Ray& r, float previousDistance);
	Intersection searchIntersection(const Ray& r,float previousDistance, std::vector<Node*>& annotated);
	void add(SmoothTriangle* prim);
};

}
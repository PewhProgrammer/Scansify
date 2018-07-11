#include "bvh.h"
#include <iterator>

typedef std::pair<float, float> f2;

using namespace rt;

rt::BVH::BVH()
{
	Root = new Node();
	Root->id = 0;
	this->NodeNum++; 
}

BBox BVH::getBounds() const {
	return Root->boundingBox; 
}



Intersection BVH::intersect(const Ray& ray, float previousBestDistance, std::vector<Node*>& a) const {

	return Root->searchIntersection(ray, previousBestDistance, a);

}
void BVH::buildIndex() {
	if (built_flag) return;
	depth = 8 + (int)std::round(( 1.3f * log10(SceneObjects.size())));
	buildTree(Root);
	built_flag = true;
}

void BVH::rebuildIndex() {
	clearAnnotation(Root);
}

rt::BVH::~BVH()
{
	free(Root);
}
void BVH::add(SmoothTriangle* s) {

	SceneObjects.push_back(s);
	Root->add(s);
}

void BVH::clearAnnotation(Node* node) {
	node->m_bAnnotated = false;
	node->m_annotationMap.clear();

	if (!node->isLeaf()) {
		clearAnnotation(node->left);
		clearAnnotation(node->right);
	}

}

void BVH::buildTree(Node* node) {

	//init node with ids
	node->left = new Node();
	node->right = new Node();

	node->left->id = NodeNum;
	this->NodeNum++;
	node->right->id = NodeNum; 
	this->NodeNum++;

	//use heuristic to split
	splitPane(node);

	//decide whether to build more 
	size_t sizeOrigin = node->objects.size();
	node->objects.clear();

	if (node->left->objects.size() > 5 && node->left->objects.size() != sizeOrigin ) {
		buildTree(node->left);
	}
	else {
		node->left->Leaf = true;
	}
	if (node->right->objects.size() > 5 && node->right->objects.size() != sizeOrigin) {
		buildTree(node->right);
	}
	else {
		node->right->Leaf = true;
	}

}

void BVH::splitPane(Node* node) {

	f2 split = splitInTheMiddle(node);
	//f2 split = SAH(node);

	for (auto iter = node->objects.begin(); iter != node->objects.end(); ++iter) {

		BBox box = (*iter)->getBounds();

		//median of prim bounds
		float mid_box = (box.min[split.first] + box.max[split.first]) * 0.5f;
		if (mid_box <= split.second) {
			node->left->add(*iter);
		}
		else {
			node->right->add(*iter);
		}

	}
}

std::pair<float, float> rt::BVH::splitInTheMiddle(Node* node)
{
	Point boxMax = node->boundingBox.max;
	Point boxMin = node->boundingBox.min;

	float x = boxMax.x - boxMin.x;
	float y = boxMax.y - boxMin.y;
	float z = boxMax.z - boxMin.z;


	if (x > y && x > z) {
		return f2(0.f, (boxMax.x + boxMin.x) * 0.5f);
	}
	else if (y > z) {
		return f2(1.f, (boxMax.y + boxMin.y) * 0.5f);
	}
	else {
		return f2(2.f, (boxMax.z + boxMin.z) * 0.5f);
	}
}

std::pair<float, float> rt::BVH::SAH(Node* node)
{
	// c = c_trav + (1 - b_e) * c_isect * ( p_b * c_b + p_a * c_a) 
	size_t _primsize = node->objects.size();
	float cost = FLT_MAX;
	float cost_ij = 0;
	int axis = 0;
	float splitat_i = -1;
	bool rand_flag = true; 

	uint16_t steps = 100;
	size_t splits = (size_t)(_primsize * 0.01f);
	if (splits == 0) {
		rand_flag = false; 
		steps = (uint16_t)_primsize;
	}

	if (node->id = 15)
		float k = 2; 

	BBox bound = node->boundingBox;

	if (_primsize < 300)
		float k = 2; 

	//inverse Surface Area
	float invSA = 1 /
		(2 * (bound.max.x - bound.min.x) +
			2 * (bound.max.y - bound.min.y) +
			2 * (bound.max.z - bound.min.z));

	//for each axis X,Y,Z
	int i = 0;
	while (i < 3) {
		for (int j = 0; j < steps; j++) {
			uint32_t countl = 0;
			size_t v1; 
			if(rand_flag)
				v1 = (rand() % splits +  j*splits )% _primsize;
			else v1 = j; 

			SmoothTriangle* prim = node->objects[v1];
			//compute centroid
			float cent_i = (prim->getBounds().min[i] + prim->getBounds().max[i]) * 0.5f;
			//compute surface area 
			for (size_t k = 0; k < _primsize; k++) {
				SmoothTriangle* primk = node->objects[k];
				float cent_k = (primk->getBounds().min[i] + primk->getBounds().max[i]) * 0.5f;
				if (cent_k <= cent_i) countl++;

			}

			//Total Surface Area = 2(Areahw) + 2(Areahl) + 2(Areawl)

			float ba = bound.max[i] - bound.min[i]; // maxlength
			float ca = cent_i - bound.min[i]; // smallest length
			float da = ba - ca; 

			float P_a = ca / ba;
			float P_b = da / ba; 

			cost_ij = (P_a * countl) + P_b * (_primsize - countl);

			if (cost_ij < cost) {
				cost = cost_ij;
				axis = i;
				splitat_i = cent_i;
			}
			if (countl == 0) {
				float k = 2; 
			}
		}
		i++;
	}

	return f2((float)axis, splitat_i);
}

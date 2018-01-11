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
Intersection BVH::intersect(const Ray& ray, float previousBestDistance) const {

	return Root->searchIntersection(ray, previousBestDistance);

}
void BVH::rebuildIndex() {
	if (built_flag) return;
	built_flag = true; 
	depth = 8 + 1.3f * log(SceneObjects.size());
	buildTree(Root);
}

rt::BVH::~BVH()
{
	free(Root);
}
void BVH::add(Point p) {

	SceneObjects.push_back(p);
	Root->add(p);
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
	float sizeOrigin = node->objects.size();
	node->objects.clear();

	if (node->left->objects.size() > 10 && node->left->objects.size() != sizeOrigin ) {
		buildTree(node->left);
	}
	else {
		node->left->Leaf = true;
	}
	if (node->right->objects.size() > 10 && node->right->objects.size() != sizeOrigin) {
		buildTree(node->right);
	}
	else {
		node->right->Leaf = true;
	}

}

void BVH::splitPane(Node* node) {

	//f2 split = splitInTheMiddle(node);
	f2 split = SAH(node);

	for (auto iter = node->objects.begin(); iter != node->objects.end(); ++iter) {

		Point p = *iter;
		
		//median of prim bounds
		float mid_box = (p[split.first] + p[split.first]) * 0.5f;
		if (mid_box <= split.second) {
			node->left->add(p);
		}
		else {
			node->right->add(p);
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


	if (x > y & x > z) {
		return f2(0, (boxMax.x + boxMin.x) * 0.5f);
	}
	else if (y > z) {
		return f2(1, (boxMax.y + boxMin.y) * 0.5f);
	}
	else {
		return f2(2, (boxMax.z + boxMin.z) * 0.5f);
	}
}

std::pair<float, float> rt::BVH::SAH(Node* node)
{
	// c = c_trav + (1 - b_e) * c_isect * ( p_b * c_b + p_a * c_a) 
	uint32_t _primsize = node->objects.size();
	float cost = FLT_MAX;
	float cost_ij = 0;
	float axis = 0;
	float splitat_i = -1;
	bool rand_flag = true; 

	uint16_t steps = 100;
	uint32_t splits = _primsize * 0.01f;
	if (splits == 0) {
		rand_flag = false; 
		steps = _primsize;
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
			int v1; 
			if(rand_flag)
				v1 = (rand() % splits +  j*splits )% _primsize;
			else v1 = j; 

			Point prim = node->objects[v1];
			//compute centroid
			float cent_i = (prim[i]) * 0.5f;
			//compute surface area 
			for (int k = 0; k < _primsize; k++) {
				Point primk = node->objects[k];
				float cent_k = (primk[i]) * 0.5f;
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

	return f2(axis, splitat_i);
}

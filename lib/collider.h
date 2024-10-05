#pragma once

#include "Common.h"
#include "vertex.h"
#include "Mat4x4f.h"

using namespace std;

typedef enum nodeTypes 
{
	sphere,
	AABB
} nodeTypes;

typedef struct 
{
	vertex_t v1;
	vertex_t v2;
	vertex_t v3;
	libMath::vector4f center;
} triangle_t;

class node {
public:
	std::vector<triangle_t*> triangles;
	nodeTypes type = sphere;
	node() {};
	virtual bool test(node* n2) = 0;
	std::vector<node*> sons;

	~node() 
	{
		for (auto& t : sons) delete t;
	}

	void addTriangle(triangle_t* triangle) 
	{
		triangles.push_back(triangle);
	}

	virtual void subdivide() = 0;
};

class sphereNode : public node 
{
public:
	float radious = 0;
	libMath::vector4f center;

	sphereNode(float radious, libMath::vector4f center) :radious(radious), center(center)
	{
		this->type = sphere;
	}

	virtual bool test(node* n2) 
	{
		sphereNode* node = (sphereNode*)n2;
		float dist = libMath::length(center - node->center);
		bool resultTest = false;
		
		if (dist < (node->radious + radious))
		{
			sphereNode* node1 = this;
			sphereNode* node2 = (sphereNode*) n2;

			if ((node1->sons.size() > 0) && (node2->sons.size() > 0)) for (auto& h1 : node1->sons) for (auto& h2 : node2->sons) resultTest |= h1->test(h2);
			else if (node1->sons.size() > 0) for (auto& h1 : node1->sons) resultTest |= node2->test(h1);
			else if (node2->sons.size() > 0) for (auto& h2 : node2->sons) resultTest |= node1->test(h2);
			else resultTest = true;
		}

		return resultTest;
	}

	void computeCenter()
	{
		float maxX = -99999999.0f, maxY = -99999999.0f, maxZ = -99999999.0f;
		float minX = 99999999.0f, minY = 99999999.0f, minZ = 99999999.0f;

		for (auto& tr : triangles) {
			maxX = std::max(maxX, (std::max(std::max(tr->v1.vertexPos.x, tr->v2.vertexPos.x), tr->v3.vertexPos.x)));
			maxY = std::max(maxY, (std::max(std::max(tr->v1.vertexPos.y, tr->v2.vertexPos.y), tr->v3.vertexPos.y)));
			maxZ = std::max(maxZ, (std::max(std::max(tr->v1.vertexPos.z, tr->v2.vertexPos.z), tr->v3.vertexPos.z)));
			minX = std::min(minX, (std::min(std::min(tr->v1.vertexPos.x, tr->v2.vertexPos.x), tr->v3.vertexPos.x)));
			minY = std::min(minY, (std::min(std::min(tr->v1.vertexPos.y, tr->v2.vertexPos.y), tr->v3.vertexPos.y)));
			minZ = std::min(minZ, (std::min(std::min(tr->v1.vertexPos.z, tr->v2.vertexPos.z), tr->v3.vertexPos.z)));
		}

		center = {(maxX + minX) / 2.0f, (maxY + minY) / 2.0f, (maxZ + minZ) / 2.0f, 1.0f};
		radious = libMath::length(libMath::make_vector4f(maxX, maxY, maxZ, 1.0f) - center);
	}

	void subdivide() override 
	{
		sphereNode* auxSons[4];
		for (int i = 0; i < 4; i++) auxSons[i] = new sphereNode(0, { 0,0,0,0 });

		for (auto tr : triangles)
		{
			if ((tr->center.x < center.x))
			{
				if (tr->center.y > center.y) auxSons[0]->triangles.push_back(tr);
				else auxSons[1]->triangles.push_back(tr);
			}
			else
			{
				if (tr->center.y > center.y) auxSons[2]->triangles.push_back(tr);
				else auxSons[3]->triangles.push_back(tr);
			}
		}

		for (auto& son : auxSons)
		{
			if (son->triangles.size() > 0)
			{
				((sphereNode*)son)->computeCenter();
				this->sons.push_back(son);

				if (son->triangles.size() > 1) son->subdivide();
			}
			else delete son;
		}
	}
};

class Collider
{
public:
	node* root = nullptr;
	std::vector<triangle_t> triangles;

	Collider() {};

	void addTriangle(vertex_t v1, vertex_t v2, vertex_t v3) 
	{
		libMath::vector4f center = {0.0f, 0.0f, 0.0f, 1.0f};

		float maxX = std::max(std::max(v1.vertexPos.x, v2.vertexPos.x), v3.vertexPos.x);
		float minX = std::min(std::min(v1.vertexPos.x, v2.vertexPos.x), v3.vertexPos.x);
		float maxY = std::max(std::max(v1.vertexPos.y, v2.vertexPos.y), v3.vertexPos.y);
		float minY = std::min(std::min(v1.vertexPos.y, v2.vertexPos.y), v3.vertexPos.y);
		float maxZ = std::max(std::max(v1.vertexPos.z, v2.vertexPos.z), v3.vertexPos.z);
		float minZ = std::min(std::min(v1.vertexPos.z, v2.vertexPos.z), v3.vertexPos.z);

		center = {(maxX + minX) / 2.0f, (maxY + minY) / 2.0f, (maxZ + minZ) / 2.0f, 1.0f};

		triangles.push_back({v1, v2, v3, center});
	}

	void subdivide() 
	{
		root->subdivide();
	}

	void update(libMath::mat4x4f M) 
	{
		if (root != nullptr) 
		{
			for (auto& t : root->triangles) delete t;
			delete root;
		}

		root = new sphereNode(0, {0, 0, 0, 0});

		for (auto& tr : triangles)
		{
			triangle_t* tr2 = new triangle_t;

			tr2->v1.vertexPos = M * tr.v1.vertexPos;
			tr2->v2.vertexPos = M * tr.v2.vertexPos;
			tr2->v3.vertexPos = M * tr.v3.vertexPos;
			tr2->center = M * tr.center;

			root->addTriangle(tr2);
		}

		((sphereNode*)root)->computeCenter();

		subdivide();
	}

	~Collider() 
	{
		delete root;
	}
	

	bool collision(Collider* coll2)
	{
		return root->test(coll2->root);
	}
};

class SphereCollider {
public:
	sphereNode* root;

	SphereCollider(float radius, libMath::vector4f center) {
		root = new sphereNode(radius, center);
	}


	bool isColliding(const SphereCollider& other) {
		libMath::vector4f diff = this->root->center - other.root->center;
		float distance = libMath::length(diff);
		return distance <= (this->root->radious + other.root->radious);
	}

	void update(libMath::mat4x4f M, libMath::vector4f center) {
		if (root != nullptr) {
			delete root;
		}

		float scaleX = libMath::length(M.rows[0]);
		float scaleY = libMath::length(M.rows[1]);
		float scaleZ = libMath::length(M.rows[2]);

		float radious = std::max({ scaleX, scaleY, scaleZ });

		root = new sphereNode(radious, center);
	}

	void updateCamera(libMath::vector4f position) {
		root->center = position;
	}

	~SphereCollider() {
		delete root;
	}

	bool collision(SphereCollider* coll2) {
		return isColliding(*coll2);
	}
};
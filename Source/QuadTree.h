#pragma once
#include "GameDev2D.h"

using namespace GameDev2D;

class Circle;

class QuadTree
{
private:
	int m_capacity = 4;
	QuadTree* m_sections[4];
	AABB m_box;
	bool m_divided = false;
public:
	QuadTree(AABB box);
	~QuadTree();
	std::vector<Circle*> m_balls;

	bool Insert(Circle* ball);
	void Divide();

	void Render(BatchRenderer& renderer);


	void Clear();

	bool CircleAABB(const AABB& box, const Vector2& pos, float r);

	std::vector<Circle*> Query(Vector2 point, float radius, std::vector<Circle*>& found);
};


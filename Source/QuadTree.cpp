#include "QuadTree.h"
#include "Ball.h"

QuadTree::QuadTree(AABB box) :
	m_box(box)
{
}

QuadTree::~QuadTree()
{
	for (int i = 0; i < 4; ++i) {
        m_sections[i] = nullptr;
    }
}

bool QuadTree::Insert(Circle* ball) {
	// Check if the ball is within the bounds of the current node
	if (!CircleAABB(m_box, ball->GetPos(), ball->GetRadius())) return false;

	if (m_divided) {
		// If the node is divided, insert into the appropriate child nodes
		for (int i = 0; i < 4; i++) {
			if (m_sections[i]->Insert(ball))
			{
				return true;
			}
		}
	}
	else {
		// If the node is not divided, add the ball to this node
		m_balls.push_back(ball);

		// If the number of balls exceeds the capacity, divide the node
		if (m_balls.size() > m_capacity) {
			Divide();
			// Reinsert existing balls into the newly divided nodes
			std::vector<Circle*> balls = m_balls;
			m_balls.clear();
			for (Circle* b : balls) {
				Insert(b);
			}
		}
	}
}

void QuadTree::Divide()
{
	Vector2 center = m_box.Center();
	Vector2 size = m_box.GetHalfDimensions();

	AABB se(center - size, center);
	m_sections[0] = new QuadTree(se);

	AABB ne({ center.x - size.x, center.y }, { center.x, center.y + size.y });
	m_sections[1] = new QuadTree(ne);

	AABB nw(center, center + size);
	m_sections[2] = new QuadTree(nw);

	AABB sw({ center.x, center.y - size.y }, { center.x + size.x, center.y });
	m_sections[3] = new QuadTree(sw);

	m_divided = true;
}

void QuadTree::Render(BatchRenderer& renderer)
{
	renderer.RenderQuad(m_box.Center(), m_box.GetDimensions(), { .5f,.5f }, ColorList::Clear, ColorList::Black, .5f);
	if (m_divided)
	{
		for (auto section : m_sections)
		{
			section->Render(renderer);
		}
	}
}



void QuadTree::Clear()
{
	m_balls.clear();
	if (m_divided) {
		for (int i = 0; i < 4; ++i) {
			m_sections[i]->Clear();
			delete m_sections[i];
			m_sections[i] = nullptr;
		}
		m_divided = false;
	}

}

bool QuadTree::CircleAABB(const AABB& box, const Vector2& pos, float r)
{
	float closestX = std::fmaxf(box.lower.x, std::fminf(pos.x, box.upper.x));
	float closestY = std::fmaxf(box.lower.y, std::fminf(pos.y, box.upper.y));

	float distanceX = pos.x - closestX;
	float distanceY = pos.y - closestY;

	return (distanceX * distanceX + distanceY * distanceY) < (r * r);
}

std::vector<Circle*> QuadTree::Query(Vector2 point, float radius, std::vector<Circle*>& found)
{
	if (!CircleAABB(m_box, point, radius))
	{
		return found;
	}

	for (Circle* ball : m_balls)
	{
		if (ball->GetPos().Distance(point) < radius + ball->GetRadius())
		{
			found.push_back(ball);
		}
	}
	if (m_divided)
	{
		for (int i = 0; i < 4; i++)
		{
			m_sections[i]->Query(point, radius, found);
		}
	}
	return found;
}

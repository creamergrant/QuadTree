#include "CollisionHell.h"
#include "QuadTree.h"
#include "Ball.h"
#include "Spline.h"

GameDev2D::CollisionHell::CollisionHell()
{
	Random r;
	for (int i = 0; i < 1000; i++)
	{
		m_balls.push_back(new Ball(GameDev2D::Vector2(r.RandomFloat(20, 1180), r.RandomFloat(20, 1180))));
	}
	AABB box;
	box.lower = { 0,0 };
	box.upper = { 1200,1200 };
	m_tree = new QuadTree(box);
}

GameDev2D::CollisionHell::~CollisionHell()
{
	delete m_tree;
	for (Ball* ball : m_balls)
	{
		delete ball;
	}
}

void GameDev2D::CollisionHell::OnUpdate(float delta)
{
	for (auto ball : m_balls)
	{
		ball->Update(delta);
	}

	if (m_usingQuadTree)
	{
		m_tree->Clear();
		for (auto ball : m_balls)
		{
			m_tree->Insert(ball);
		}
		for (int i = 0; i < m_balls.size(); i++)
		{
			std::vector<Circle*> results;
			m_tree->Query(m_balls[i]->GetPos(), m_balls[i]->GetRadius() * 2, results);
			for (Circle* c : results)
			{
				Vector2 dir1 = m_balls[i]->GetDir();
				Vector2 pos1 = m_balls[i]->GetPos();
				if (Ball* b = dynamic_cast<Ball*>(c))
				{
					if (m_balls[i]->GetPos().Distance(b->GetPos()) < m_balls[i]->GetRadius() + b->GetRadius())
					{
						Vector2 dir2 = b->GetDir();
						Vector2 pos2 = b->GetPos();
						BallOnBallAction(dir1, pos1, dir2, pos2, m_balls[i]->GetRadius(), b->GetRadius(), m_balls[i]->GetMass(), b->GetMass());
						m_balls[i]->SetDir(dir1.Normalized());
						m_balls[i]->SetPos(pos1);
						b->SetDir(dir2.Normalized());
						b->SetPos(pos2);
					}
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < m_balls.size(); i++)
		{
			for (Ball* b : m_balls)
			{
				if (m_balls[i]->GetPos().Distance(b->GetPos()) < m_balls[i]->GetRadius() + b->GetRadius())
				{
					Vector2 dir1 = m_balls[i]->GetDir();
					Vector2 dir2 = b->GetDir();
					Vector2 pos1 = m_balls[i]->GetPos();
					Vector2 pos2 = b->GetPos();
					BallOnBallAction(dir1, pos1, dir2, pos2, m_balls[i]->GetRadius(), b->GetRadius(), m_balls[i]->GetMass(), b->GetMass());
					m_balls[i]->SetDir(dir1.Normalized());
					b->SetDir(dir2.Normalized());
					m_balls[i]->SetPos(pos1);
					b->SetPos(pos2);
				}
			}
		}
	}
}

void GameDev2D::CollisionHell::OnRender(BatchRenderer& batchRenderer)
{
	//Render your Sprites and SpriteFonts here
	for (auto ball : m_balls)
	{
		ball->Render(batchRenderer);
	}
	m_tree->Render(batchRenderer);
}

void GameDev2D::CollisionHell::OnKeyEvent(KeyCode keyCode, KeyState keyState)
{
	if (keyCode == KeyCode::Q && keyState == KeyState::Down)
	{
		m_usingQuadTree = !m_usingQuadTree;
	}
}

void GameDev2D::CollisionHell::OnMouseButtonEvent(MouseButton button, MouseButtonState state, float mouseX, float mouseY)
{
}

void GameDev2D::CollisionHell::OnMouseMovedEvent(float mouseX, float mouseY)
{
}

AABB GameDev2D::CollisionHell::CreateAABBOnBall(Ball* ball, float size)
{
	Vector2 center = ball->GetPos();
	Vector2 half = { size / 2, size / 2 };
	return AABB(center - half, center + half);
}

void GameDev2D::CollisionHell::BallOnBallAction(Vector2& v1, Vector2& p1, Vector2& v2, Vector2& p2, float r1, float r2, float m1, float m2)
{
	Vector2 collisionVector = p2 - p1;
	double distance = (collisionVector).Length();
	double overlap = (r1 + r2) - distance;

	if (overlap > 0) {
		Vector2 displacement = (collisionVector * (overlap / 2)).Normalized();
		p1 -= displacement;
		p2 += displacement;
	}

	Vector2 n = (p2 - p1).Normalized();

	Vector2 rel = v1 - v2;

	float reldot = rel.DotProduct(n);
	Vector2 relnorm = n * reldot;

	double j = -(2 * reldot) / (1 / m1 + 1 / m2);
	Vector2 J = n * j;

	v1 += J / m1;
	v2 -= J / m2;
}

bool GameDev2D::CollisionHell::CircleLineSegment(Vector2& circlePos, Vector2& vel, float radius, Vector2 start, Vector2 end)
{
	Vector2 line = end - start;

	Vector2 circle = circlePos - start;

	float projection = (circle.DotProduct(line)) / line.LengthSquared();

	projection = std::fmaxf(0, std::fminf(1, projection));

	Vector2 closePoint = start + line * projection;
	float dist = (closePoint - circlePos).Length();

	if (dist <= radius)
	{
		Vector2 normal = circlePos - closePoint;
		normal.Normalize();

		float dot = vel.DotProduct(normal);
		vel = vel - 2 * dot * normal;

		float overlap = radius - dist;
		circlePos += normal * overlap;
		return true;
	}
	return false;
}



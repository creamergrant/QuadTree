#include "Ball.h"

Ball::Ball(GameDev2D::Vector2 pos) :
	Circle(pos)
{
	GameDev2D::Random r;
	m_color = GameDev2D::ColorList::White;
	m_dir = GameDev2D::Vector2(r.RandomFloat(-1, 1), r.RandomFloat(-1, 1));
	m_radius = 4;//r.RandomFloat(1, 8);
	m_mass = m_radius;
}

Ball::~Ball()
{
}

void Ball::Update(float DeltaTime)
{
	m_pos += m_dir * m_speed * DeltaTime;
	if (m_pos.x - m_radius < 0 || m_pos.x + m_radius>1200)
	{
		m_dir.x *= -1;
	}
	if (m_pos.y - m_radius < 0 || m_pos.y + m_radius>1200)
	{
		m_dir.y *= -1;
	}
}

void Ball::Render(GameDev2D::BatchRenderer& renderer)
{
	Circle::Render(renderer);
}


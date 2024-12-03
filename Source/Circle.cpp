#include "Circle.h"

Circle::Circle(GameDev2D::Vector2 pos) :
	m_pos(pos)
{
	GameDev2D::Random r;
	m_color = GameDev2D::ColorList::White;
	m_radius = r.RandomFloat(1, 8);
}

Circle::Circle(Vector2 pos, float radius):
	m_pos(pos),
	m_radius(radius)
{
}

Circle::~Circle()
{
}

void Circle::Update(float DeltaTime)
{
	
}

void Circle::Render(GameDev2D::BatchRenderer& renderer)
{
	renderer.RenderCircle(m_pos, m_radius, m_color);
}


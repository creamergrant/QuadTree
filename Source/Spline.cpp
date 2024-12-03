#include "Spline.h"
#include "Circle.h"
#include "QuadTree.h"

GameDev2D::Spline::Spline(QuadTree* qt) :
	m_qt(qt)
{

}

GameDev2D::Spline::~Spline()
{
}

void GameDev2D::Spline::Update(float delta)
{
	m_curvePoints.clear();
	for (int i = 0; i < m_points.size(); i++)
	{
		CatmulRom(i);
	}
	for (Vector2 point : m_curvePoints)
	{
		m_qt->Insert(new Circle(point, 8.0f));
	}
}

void GameDev2D::Spline::Render(BatchRenderer& renderer)
{
	for (int i = 0; i < m_curvePoints.size(); i++)
	{
		renderer.RenderCircle(m_curvePoints[i], 4, ColorList::Clear, ColorList::Black, 2);
		renderer.RenderLineStrip(m_curvePoints, ColorList::Black, 2);
	}
}

void GameDev2D::Spline::OnMouseButtonEvent(MouseButton button, MouseButtonState state, float mouseX, float mouseY)
{
	if (button == MouseButton::Left && state == MouseButtonState::Released)
	{
		if (m_points.size() < 2)
		{
			m_points.push_back({ mouseX, mouseY });
		}
		else if(m_curvePoints.size() < 150)
		{
			m_points[m_points.size() - 1] = { mouseX, mouseY };
			m_points.push_back(m_points[0]);
		}
	}
}

void GameDev2D::Spline::CatmulRom(int index)
{
	Vector2 p0 = m_points[GetIndex(index + 0)];
	Vector2 p1 = m_points[GetIndex(index + 1)];
	Vector2 p2 = m_points[GetIndex(index + 2)];
	Vector2 p3 = m_points[GetIndex(index + 3)];

	float t0 = 0.0f;
	float t1 = GetT(t0, p0, p1);
	float t2 = GetT(t1, p1, p2);
	float t3 = GetT(t2, p2, p3);

	for (float t = t1; t < t2; t += ((t2 - t1) / m_amtPoints))
	{
		Vector2 A1 = (t1 - t) / (t1 - t0) * p0 + (t - t0) / (t1 - t0) * p1;
		Vector2 A2 = (t2 - t) / (t2 - t1) * p1 + (t - t1) / (t2 - t1) * p2;
		Vector2 A3 = (t3 - t) / (t3 - t2) * p2 + (t - t2) / (t3 - t2) * p3;

		Vector2 B1 = (t2 - t) / (t2 - t0) * A1 + (t - t0) / (t2 - t0) * A2;
		Vector2 B2 = (t3 - t) / (t3 - t1) * A2 + (t - t1) / (t3 - t1) * A3;

		Vector2 C = (t2 - t) / (t2 - t1) * B1 + (t - t1) / (t2 - t1) * B2;

		if (m_curvePoints.size() == 0 || m_curvePoints[m_curvePoints.size() - 1] != C)
		{
			m_curvePoints.push_back(C);
		}
	}
}

float GameDev2D::Spline::GetT(float t, Vector2 p0, Vector2 p1)
{
	float a = std::powf((p1.x - p0.x), 2.0f) + std::powf(p1.y - p0.y,2.0f);
	float b = std::powf(a, .5f);
	float c = std::pow(b, .5f);

	return c + t;
}

int GameDev2D::Spline::GetIndex(int index)
{
	return index % m_points.size();
}

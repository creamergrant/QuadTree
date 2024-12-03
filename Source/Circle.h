#pragma once
#include "GameDev2D.h"
using namespace GameDev2D;

class Circle
{
protected:
	Vector2 m_pos;
	float m_radius = 4;
	Color m_color;
public:
	Circle(Vector2 pos);
	Circle(Vector2 pos, float radius);
	virtual ~Circle();

	void Update(float DeltaTime);
	void Render(BatchRenderer& renderer);

	Vector2 GetPos() { return m_pos; }
	void SetPos(Vector2 pos) { m_pos = pos; }

	float GetRadius() { return m_radius; }

};
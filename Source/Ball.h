#pragma once
#include "GameDev2D.h"
#include "Circle.h"
using namespace GameDev2D;

class Ball : public Circle
{
private:
	Vector2 m_dir;
	float m_speed = 50;
	float m_mass = 1;
public:
	Ball(Vector2 pos);
	virtual ~Ball();

	void Update(float DeltaTime);
	void Render(BatchRenderer& renderer);

	Vector2 GetDir() { return m_dir; }
	void SetDir(Vector2 dir) { m_dir = dir; }

	float GetMass() { return m_mass; }

};
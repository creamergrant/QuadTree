#pragma once

#include <GameDev2D.h>

class Ball;
class QuadTree;

namespace GameDev2D
{
class Spline;
	class CollisionHell
	{
	private:
		//Member variables GO HERE 
		std::vector<Ball*> m_balls;

		QuadTree* m_tree;

		bool m_usingQuadTree = false;

		Spline* m_spline;

	public:
		CollisionHell();
		~CollisionHell();

		//Write game logic in the Update method
		void OnUpdate(float delta);

		//Render game objects in the Draw method
		void OnRender(BatchRenderer& batchRenderer);

		//Input event methods
		void OnKeyEvent(KeyCode keyCode, KeyState keyState);
		void OnMouseButtonEvent(MouseButton button, MouseButtonState state, float mouseX, float mouseY);
		void OnMouseMovedEvent(float mouseX, float mouseY);
	private:
		AABB CreateAABBOnBall(Ball* ball, float size);

		void BallOnBallAction(Vector2& v1, Vector2& p1, Vector2& v2, Vector2& p2, float r1, float r2, float m1, float m2);

		bool CircleLineSegment(Vector2& circlePos, Vector2& vel, float radius, Vector2 start, Vector2 end);
	};
}
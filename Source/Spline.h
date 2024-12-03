#pragma once
#include "GameDev2D.h"

class QuadTree;

namespace GameDev2D
{
	class Spline
	{
	private:
		std::vector<Vector2> m_points;
		std::vector<Vector2> m_curvePoints;
		QuadTree* m_qt;
		int m_amtPoints = 15;
	public:
		Spline(QuadTree* qt);
		~Spline();

		void Update(float delta);
		void Render(BatchRenderer& renderer);

		void OnMouseButtonEvent(MouseButton button, MouseButtonState state, float mouseX, float mouseY);

		void CatmulRom(int index);

		float GetT(float t, Vector2 p0, Vector2 p1);

		int GetIndex(int index);

		void GetLine(Vector2 position, Vector2& l1, Vector2& l2)
		{
			int index = 0;
			for (int i = 0; i < m_curvePoints.size(); i++)
			{
				if (position == m_curvePoints[i])
				{
					index = i;
					break;
				}
			}
			l1 = m_curvePoints[GetIndex(index - 1)];
			l2 = m_curvePoints[GetIndex(index + 1)];
		}
	};
}


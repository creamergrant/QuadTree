#include "Game.h"
#include "CollisionHell.h"

namespace GameDev2D
{
	Game::Game()
	{
		m_ballsim = new CollisionHell();
	}

	Game::~Game()
	{
		delete m_ballsim;
	}

	void Game::OnUpdate(float delta)
	{
		m_ballsim->OnUpdate(delta);
	}

	void Game::OnRender(BatchRenderer& batchRenderer)
	{
		batchRenderer.BeginScene();

		m_ballsim->OnRender(batchRenderer);

		batchRenderer.EndScene();
	}

	void Game::OnKeyEvent(KeyCode keyCode, KeyState keyState)
	{
		m_ballsim->OnKeyEvent(keyCode, keyState);
	}

	void Game::OnMouseButtonEvent(MouseButton button, MouseButtonState state, float mouseX, float mouseY)
	{
		m_ballsim->OnMouseButtonEvent(button, state, mouseX, mouseY);
	}

	void Game::OnMouseMovedEvent(float mouseX, float mouseY)
	{
		m_ballsim->OnMouseMovedEvent(mouseX, mouseY);
	}
}
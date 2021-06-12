#include "..\Engine\Engine.h"
#include <exception>

struct Vector2D
{
	int x;
	int y;
};

void DrawRect(Engine& engine, Vector2D pos, int width, int height, Engine::Color color) noexcept
{
	for (int inx = pos.x; inx <= pos.x + width; inx++)
	{
		for (int iny = pos.y; iny <= pos.y + height; iny++)
		{
			engine.SetPixel(inx, iny, color);
		}
	}
}

class Score
{
public:
	Score(Engine& engine, Vector2D pos, int score)
		:
		engine(engine),
		scorepos(pos),
		score(score)
	{}
	void Draw() noexcept
	{
		const int startposx = scorepos.x - ((scoredim + padding)* score / 2);
		for (int inscore = 0; inscore < score; inscore++)
		{
			DrawRect(engine, { startposx + ((scoredim + padding)* inscore), scorepos.y }, scoredim, scoredim, scorecolor);
		}
	}
	void AddScore() noexcept
	{
		score++;
	}
	static int GetSize() noexcept
	{
		return scoredim;
	}
private:
	Engine& engine;
	static constexpr int scoredim = 10;
	static constexpr Engine::Color scorecolor = { 255,255,255 };
	static constexpr int padding = 10;
	int score = 1;
	Vector2D scorepos;
};

class PongPlayer
{
public:
	PongPlayer(Engine& engine, Vector2D pos, Vector2D scorepos)
		:
		engine(engine),
		pos(pos),
		score(engine, scorepos, 1u)
	{}
	int GetX() const noexcept
	{
		return pos.x;
	}
	int GetY() const noexcept
	{
		return pos.y;
	}
	static int GetHeight() noexcept
	{
		return height;
	}
	static int GetWidth() noexcept
	{
		return width;
	}

	void Update(int delta) noexcept
	{
		if (pos.y + delta + height < engine.GetHeight() && pos.y + delta > 0)
		{
			pos.y += delta;
		}
	}
	void Draw() noexcept
	{
		DrawRect(engine, pos, width, height, color);
		score.Draw();
	}
	void AddScore() noexcept
	{
		score.AddScore();
	}
	void Reset() noexcept
	{
		pos.y = engine.GetHeight() / 2 - height / 2;
	}
private:
	Engine& engine;
	static constexpr int width = 20, height = 100;
	static constexpr Engine::Color color = { 255,255,255 };
	Score score;
	Vector2D pos;
};

class PongBall
{
public:
	PongBall(Engine& engine, PongPlayer& p1, PongPlayer& p2)
		:
		engine(engine),
		pos({ engine.GetWidth() / 2 - size / 2, engine.GetHeight() / 2 - size / 2 }),
		vel{ 1, 1 },
		player1(p1),
		player2(p2)
	{}
	static int GetSize() noexcept
	{
		return size;
	}
	void Update(int delta_p1, int delta_p2) noexcept
	{
		pos.x += vel.x * speed;
		pos.y += vel.y * speed;
		if (pos.x + size >= engine.GetWidth() || pos.x < 0)
		{
			if (vel.x > 0)
			{
				player2.AddScore();
			}
			else
			{
				player1.AddScore();
			}
			gameisover = true;
			Reset();
		}
		if (pos.y + size >= engine.GetHeight() || pos.y < 0)
		{
				vel.y *= -1;
			pos.y += vel.y * speed;
		}
		if (pos.x <= player1.GetX() + player1.GetWidth() && (pos.y + size > player1.GetY() && pos.y < player1.GetY() + player1.GetHeight()))
		{
			if (delta_p1 > 0)
			{         
				if (vel.y > 0)
				{
					vel.y = 1;
				}
				else
				{
					vel.y = -1;
				}
			}
			vel.x = -vel.x;
			pos.x = player1.GetX() + player1.GetWidth() + 1;
			return;
		}
		if (pos.x + size >= player2.GetX() && (pos.y + size > player2.GetY() && pos.y < player2.GetY() + player2.GetHeight()))
		{
			if (delta_p2 > 0)
			{
				if (vel.y > 0)
				{
					vel.y = 1;
				}
				else
				{
					vel.y = -1;
				}
			}
			vel.x = -vel.x;
			pos.x = player2.GetX() - size - 1;
		}
	}
	void Draw() noexcept
	{
		DrawRect(engine, pos, size, size, color);
	}
	bool GetGameIsOver() const noexcept
	{
		return gameisover;
	}
	void SetGameIsOver() noexcept
	{
		gameisover = false;
	}
private:
	void Reset() noexcept
	{
		pos = { engine.GetWidth() / 2, engine.GetHeight() / 2 };
		vel = { -vel.x, 0 };
		player1.Reset();
		player2.Reset();
	}
private:
	Engine& engine;
	static constexpr int speed = 8;
	static constexpr int size = 20;
	static constexpr Engine::Color color = { 255,255,255 };
	PongPlayer& player1;
	PongPlayer& player2;
	bool gameisover = true;
	Vector2D pos;
	Vector2D vel;
};

int __stdcall wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPWSTR pCmdLine, _In_ int)
{
	Engine::Init();
	try
	{
		Engine engine("Engine", 800, 600);
		// configurations
		const int startingy = engine.GetHeight() / 2 - PongPlayer::GetHeight();
		constexpr int deltaspeed = 9;

		// actors
		PongPlayer p1(engine, { 20, startingy }, {engine.GetWidth() / 2, Score::GetSize()});
		PongPlayer p2(engine, { engine.GetWidth() - 40, startingy }, { engine.GetWidth() / 2, engine.GetHeight() - Score::GetSize() * 2 });
		PongBall ball(engine, p1, p2);
		p1.Reset();
		p2.Reset();

		// delta
		int delta_p1 = 0;
		int delta_p2 = 0;

		while (engine.Run() > 0)
		{
			engine.BeginFrame();
			// Update
			// -------------------------------------------------------
			if (!ball.GetGameIsOver())
			{
				if (engine.KeyIsPressed('W')) { delta_p1 = -1 * deltaspeed; }
				else if (engine.KeyIsPressed('S')) { delta_p1 = 1 * deltaspeed; }
				else { delta_p1 = 0; }

				if (engine.KeyIsPressed(VK_UP)) { delta_p2 = -1 * deltaspeed; }
				else if (engine.KeyIsPressed(VK_DOWN)) { delta_p2 = 1 * deltaspeed; }
				else { delta_p2 = 0; };

				p1.Update(delta_p1);
				p2.Update(delta_p2);
				ball.Update(delta_p1, delta_p2);
			}
			else
			{
				if (engine.KeyIsPressed(VK_SPACE))
				{
					ball.SetGameIsOver();
				}
			}
			// Draw
			// -------------------------------------------------------
			p1.Draw();
			p2.Draw();
			ball.Draw();
			engine.EndFrame();
		}
	}
	catch (const std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Exception caught:", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(NULL, "Unknown exception", "Exception caught:", MB_OK | MB_ICONEXCLAMATION);
	}
	return 0;
}
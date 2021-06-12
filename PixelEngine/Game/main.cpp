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
		const int startposx = scorepos.x - ((scoredim + padding) * score);
		for (int inscore = 0; inscore <= score; inscore++)
		{
			DrawRect(engine, { startposx + (scoredim + padding) * inscore + 2, scorepos.y }, scoredim, scoredim, scorecolor);
		}
	}
private:
	Engine& engine;
	static constexpr int scoredim = 10;
	static constexpr Engine::Color scorecolor = { 255,255,255 };
	static constexpr int padding = 10;
	int score;
	Vector2D scorepos;
};

class PongRectangle
{
public:
	PongRectangle(Engine& engine, Vector2D pos)
		:
		engine(engine),
		pos(pos)
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
	}
private:
	Engine& engine;
	static constexpr int width = 20, height = 100;
	static constexpr Engine::Color color = { 255,255,255 };
	Vector2D pos;
};

class PongBall
{
public:
	PongBall(Engine& engine, Vector2D pos)
		:
		engine(engine),
		pos(pos),
		vel{ 1, 1 }	
	{}
	static int GetSize() noexcept
	{
		return size;
	}
	void Update() noexcept
	{
		pos.x += vel.x * speed;
		pos.y += vel.y * speed;
		if (pos.x + size >= engine.GetWidth() || pos.x < 0)
		{
			vel.x *= -1;
			pos.x += vel.x * speed;
		}
		if (pos.y + size >= engine.GetHeight() || pos.y < 0)
		{
			vel.y *= -1;
			pos.y += vel.y * speed;
		}
	}
	void Draw() noexcept
	{
		DrawRect(engine, pos, size, size, color);
	}
private:
	Engine& engine;
	static constexpr int speed = 8;
	static constexpr int size = 20;
	static constexpr Engine::Color color = { 255,255,255 };
	Vector2D pos;
	Vector2D vel;
};

int __stdcall wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ LPWSTR pCmdLine, _In_ int)
{
	Engine::Init();
	try
	{
		Engine engine("Hello", 800, 600);
		const int startingy = engine.GetHeight() / 2 - PongRectangle::GetHeight();
		constexpr int deltaspeed = 4;

		PongRectangle p1(engine, { 20, startingy });
		PongRectangle p2(engine, { engine.GetWidth() - 40, startingy });
		PongBall ball(engine, { engine.GetWidth() / 2 - PongBall::GetSize(), engine.GetHeight() / 2 - PongBall::GetSize() });
		Score score(engine, { engine.GetWidth() / 2, 50 }, 3);

		int delta_p1 = 0;
		int delta_p2 = 0;

		while (engine.Run() > 0)
		{
			engine.BeginFrame();
			// Update
			if (engine.KeyIsPressed('W')) { delta_p1 = -1 * deltaspeed; }
			else if (engine.KeyIsPressed('S')) { delta_p1 = 1 * deltaspeed; }
			else { delta_p1 = 0; }

			if (engine.KeyIsPressed(VK_UP)) { delta_p2 = -1 * deltaspeed; }
			else if (engine.KeyIsPressed(VK_DOWN)) { delta_p2 = 1 * deltaspeed; }
			else { delta_p2 = 0; }

			p1.Update(delta_p1);
			p2.Update(delta_p2);
			ball.Update();

			// Draw
			p1.Draw();
			p2.Draw();
			ball.Draw();
			score.Draw();
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
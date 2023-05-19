#pragma once
#include "SDL.h"
#include "structs.h"

class Game
{
public:
	Game();
	~Game();
	bool Initialize();
	void Shutdown();
	void GameLoop();

private:
	void ProcessInputs();
	void Update();
	void GenerateOutput();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	bool mIsRunning;

	Vector2D mPaddlePos, mBallPos, mBallVel;

	const int thickness = 15;
	const int mWindowX = 1024;
	const int mWindowY = 768;

	Uint32 mTicksCount;
	int mPaddleDir;
	int mPaddleW = thickness;
	int mPaddleH = thickness * 10;

};


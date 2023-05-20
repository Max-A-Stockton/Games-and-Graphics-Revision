#include "Game.h"

//Constructor
Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;
	mIsRunning = true;
	
	
	mTicksCount = 0;
	mPaddleDir = 0;
	mBallPos.x = mWindowX / 2;
	mBallPos.y = mWindowY / 2;
	mBallVel.x = 200.0f;
	mBallVel.y = 235.0f;

	mPaddlePos.x = 50; //Roughly on left side of the screen
	mPaddlePos.y = mWindowY / 2;
}

//Deconstructor
Game::~Game()
{
	Shutdown();
}

bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	
	if (sdlResult != 0) //If video not Initialized
	{
		SDL_Log("Unable to Initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("A Game Thing", 100, 100, mWindowX, mWindowY, 0); // (Window Name, Top left X coordinate, Top Left y coordinate, Width of window, Height of window, flags)

	if (!mWindow) //If mWindow = nullptr and therefore not initialized
	{
		SDL_Log("Unable to Initialize SDL: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); //(Window, index, flags)

	if (!mRenderer) //If mRenderer = nullptr and therefore not initialized
	{
		SDL_Log("Unable to Initialize SDL: %s", SDL_GetError());
		return false;
	}

	return true;
}

void Game::ProcessInputs()
{
	SDL_Event event;

	//Instead of checking for specific keyboard inputs we will grab state of entire keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);

	//If escape pressed
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}
	else if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
	}

	while (SDL_PollEvent(&event)) //While events are in queue
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}
}
void Game::Update()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)) //Ensures frame doesn't complete too early
		;

		float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f; //Delta time is difference in ticks from last frame

		mTicksCount = SDL_GetTicks(); //Update ticks counts

		//Clamp Delta Time value
		if (deltaTime > 0.05f)
			deltaTime = 0.05f;

		//Update World
		//Update paddle
		//Paddle Movement
		if (mPaddleDir != 0)
		{
			mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
			if (mPaddlePos.y < thickness) //if (mPaddlePos.y < (mPaddleH / 2.0f + thickness)) 
			{
				mPaddlePos.y = thickness;
			}
			else if (mPaddlePos.y > (768.0f - mPaddleH))
			{
				mPaddlePos.y = 768.0f - mPaddleH;
			}
			//This isn't a very good way of doing collision, but without the implementation of AABB this is the next best thing
		}

		//Update Ball Velocity
		//Top Wall
		if (mBallPos.y <= thickness && mBallVel.y < 0.0f) //If ball bounces off top wall and is moving towards the top wall
		{
			mBallVel.y *= -1;
		}

		//Bottom Wall
		if (mBallPos.y >= mWindowY - thickness && mBallVel.y > 0.0f) //If ball bounces off bottom wall and is moving towards the bottom wall
		{
			mBallVel.y *= -1;
		}

		//Right Wall
		if (mBallPos.x >= mWindowX - thickness && mBallVel.x > 0.0f) //If ball bounces off bottom wall and is moving towards the bottom wall
		{
			mBallVel.x *= -1;
		}

		//Paddle
		int diff = (mPaddlePos.y - mBallPos.y); //Something is very wrong here, off-set collision to top of paddle
		diff = (diff > 0.0f) ? diff : -diff;
		if (diff <= (mPaddleH + 150.0f) / 2.0f  && mBallPos.x <= mPaddlePos.x + mPaddleW && mBallPos.x >= 20.0f && mBallVel.x < 0.0f)
		{
			mBallVel.x *= -1.1f;
		}

		//Update Ball Position using Velocity and Delta Time;
		mBallPos.x += mBallVel.x * deltaTime;
		mBallPos.y += mBallVel.y * deltaTime;

		//Ball Reset
		if (mBallPos.x <= 0.0f)
		{
			mBallPos.x = mWindowX / 2;
			mBallPos.y = mWindowY / 2;
			mBallVel.x = 200.0f;
			mBallVel.y = 235.0f;
		}
}

void Game::GenerateOutput()
{
	//Clear Back Buffer
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255); //(Renderer, R, G, B, A)
	SDL_RenderClear(mRenderer);

	//Where rendering a game scene would be
	//Draw Rectangle
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); 
	SDL_Rect wall{
		0, 0, mWindowX, thickness //(Top Left X, Top Left Y, Width, Height)
	}; //Top Wall

	SDL_Rect wall2{
		0, mWindowY - thickness, mWindowX, thickness
	}; //Bottom Wall

	SDL_Rect wall3{
		mWindowX - 24, 0, mWindowX - 24, mWindowY
	}; //Right Wall

	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2), static_cast<int>(mBallPos.y - thickness / 2), thickness, thickness
	}; //Ball

	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x - thickness / 2), static_cast<int>(mPaddlePos.y - thickness / 2), mPaddleW, mPaddleH
	}; //Paddle

	SDL_RenderFillRect(mRenderer, &wall);
	SDL_RenderFillRect(mRenderer, &wall2);
	SDL_RenderFillRect(mRenderer, &wall3);
	SDL_RenderFillRect(mRenderer, &ball);
	SDL_RenderFillRect(mRenderer, &paddle);

	//Swap front and back buffers
	SDL_RenderPresent(mRenderer);

}


//GameLoop
void Game::GameLoop()
{
	while (mIsRunning) //Game Loop
	{
		ProcessInputs(); //1. Process Inputs
		Update(); //2. Update Game State
		GenerateOutput(); //3. Output
	}
}
	
void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}





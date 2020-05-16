#include "Game.h"
#include "Utility.h"
#include <cmath>
#include <ctime>

using namespace std;

Game* Game::pGame = 0;

Game* Game::getInstance()
{
	if (pGame == 0)
	{
		pGame = new Game();
	}
	return pGame;
}

Game::Game() :
	sysTime(0.0),
	gameTime(0.0),
	pShip(0),
	shipDead(false),
	upKeyHeld(false),
	leftKeyHeld(false),
	rightKeyHeld(false),
	fireKeyHeld(false),
	timeOfDeath(0.0)
{
	// Initialize SDL variables
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Utility::report("SDL Error", "Couldn't initialize");
		readyToPlay = false;
	}
	else
	{
		// Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			Utility::report("SDL Error", "Linear texture filtering not enabled");
		}

		// Create window
		pSdlWindow = SDL_CreateWindow("Dark Matter Project",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (pSdlWindow == 0)
		{
			Utility::report("SDL Error", "Couldn't create window");
			readyToPlay = false;
		}
		else
		{
			// Create vertical synced renderer for window
			pSdlRenderer = SDL_CreateRenderer(pSdlWindow, -1,
				SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			if (pSdlRenderer == 0)
			{
				Utility::report("SDL Error", "Renderer couldn't be created");
				readyToPlay = false;
			}
			else
			{
				// Initialize renderer color
				SDL_SetRenderDrawColor(pSdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) && imgFlags))
				{
					Utility::report("SDL Image Error", "Couldn't initialize");
					readyToPlay = false;
				}
			}
		}
	}
}

bool Game::createGameObjects()
{
	//Loading success flag
	bool success = true;

	// Create player's ship
	pShip = new Ship();
	pShip->overrideHeading(0.0);
	pShip->overridePosition(Vector2D(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	pShip->setSpeed(0.0);
	pShip->setAxialAccel(10.0);
	pShip->overrideAge(0.0);

	// Create a comet
	Comet* pComet = new Comet();
	pComet->overrideHeading(225);
	pComet->overridePosition(Vector2D((3 * SCREEN_WIDTH) / 4, (3 * SCREEN_HEIGHT) / 4));
	pComet->setSpeed(5);
	pComet->overrideAge(0.0);
	pCometList.push_back(pComet);

	// Load game object textures
	if (!pShip->accessTexture().loadFromFile(pSdlRenderer, "26_motion/ship.bmp"))
	{
		printf("Failed to load ship texture!\n");
		success = false;
	}

	if (!pCometList[0]->accessTexture().loadFromFile(pSdlRenderer, "26_motion/comet.bmp"))
	{
		printf("Failed to load comet texture!\n");
		success = false;
	}

	return success;
}

void Game::play()
{
	if (readyToPlay)
	{
		if (!createGameObjects())
		{
			Utility::report("Game Error", "Game couldn't create game objects");
			Utility::pressKey();
			return;
		}

		bool quit = false;

		// Game time
		double gameTime = 0.0;

		// Clock tick
		std::clock_t tick = std::clock();

		while(!quit)
		{
			// Update times
			double dt = (std::clock() - tick) / (double) CLOCKS_PER_SEC;
			tick = std::clock();
			
			// Update game time
			gameTime += dt;

			// Poll for events
			quit = pollEvents();

			// Propagate Game objects
			if (!propagateGameObjects(dt))
			{
				Utility::report("Game Error", "Game couldn't propagate game objects");
				Utility::pressKey();
			}

			//// Handle collisions
			//if (!handleCollisions())
			//{
			//	Utility::report("Game Error", "Game couldn't handle collisions");
			//	Utility::pressKey();
			//}

			// Respawn ship if dead for longer than 3 seconds
			if (shipDead)
			{
				if (timeOfDeath + 3.0 < gameTime)
				{
					if (pShip == 0)
					{
						try
						{
							pShip = new Ship();
							pShip->overridePosition(Vector2D(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
							pShip->overrideHeading(0.0);
							pShip->setSpeed(0.0);
							pShip->setAxialAccel(10.0);
							pShip->overrideAge(0.0);
						}
						catch (exception ex)
						{
							Utility::report("Game Error", "Game couldn't create new ship");
							Utility::pressKey();
						}
					}
					shipDead = false;
				}
			}

			// Throttle game speed 
			//double cycleTime = (std::clock() - tick) / (double) CLOCKS_PER_SEC;
			//Utility::sleep(0.0 > 32.0 - cycleTime ? 0.0 : 32.0 - cycleTime);
		}
	}
	else
	{
		Utility::report("Game Error", "Game couldn't initialize");
		Utility::pressKey();
	}
}


bool Game::pollEvents()
{
	bool quit = false;

	// Get Key Events
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		// User requests quit
		switch (e.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:

			switch (e.key.keysym.sym)
			{
			case SDLK_UP:
				upKeyHeld = true;
				break;
			case SDLK_LEFT:
				leftKeyHeld = true;
				break;
			case SDLK_RIGHT:
				rightKeyHeld = true;
				break;
			case SDLK_SPACE:
				fireKeyHeld = true;
				break;
			}
			break;

		case SDL_KEYUP:

			switch (e.key.keysym.sym)
			{
			case SDLK_UP:
				upKeyHeld = false;
				break;
			case SDLK_LEFT:
				leftKeyHeld = false;
				break;
			case SDLK_RIGHT:
				rightKeyHeld = false;
				break;
			case SDLK_SPACE:
				fireKeyHeld = false;
				break;
			}
			break;
		}
	}

	return quit;
}

bool Game::handleCollisions()
{
	bool success = false;

	try
	{
		// Collisions with ship
		for (std::vector<Comet*>::iterator cItr = pCometList.begin(); cItr != pCometList.end();)
		{
			if (Utility::gameObjectsOverlap(pShip, *cItr))
			{
				timeOfDeath = gameTime;
				shipDead = true;
				delete pShip;
				pShip = 0;
			}
		}

		// Game object collisions
		for (std::vector<Shot*>::iterator sItr = pShotList.begin(); sItr != pShotList.end();)
		{
			for (std::vector<Comet*>::iterator cItr = pCometList.begin(); cItr != pCometList.end();)
			{
				// If shot hit a comet, destroy both the shot and comet
				if (Utility::gameObjectsOverlap(*sItr, *cItr))
				{
					// Destroy shot
					delete* sItr;
					sItr = pShotList.erase(sItr);

					// If comet was destroyed, replace comet 
					// with new comets it spawned (if any)
					std::vector<Comet*> comets = (*cItr)->explode();
					delete* cItr;
					cItr = pCometList.erase(cItr);
					cItr = pCometList.insert(cItr, comets.begin(), comets.end());
					break;
				}
				else
				{
					++sItr;
					++cItr;
				}
			}
		}
	}
	catch (exception ex)
	{
		success = false;
	}

	return success;
}

bool Game::propagateGameObjects(double dt)
{
	bool success = true;

	// Clear game area
	SDL_SetRenderDrawColor(pSdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(pSdlRenderer);

	// Propagate/Render Ship
	if (!shipDead)
	{
		// Turn ship left
		if (leftKeyHeld)
			pShip->accelLeft();

		// Turn ship right
		if (rightKeyHeld)
			pShip->accelRight();

		// Apply acceleration
		if (upKeyHeld)
			pShip->accelAxially(true);
		else
			pShip->accelAxially(false);

		// fire weapon
		if (fireKeyHeld)
		{
			Shot* pShot = pShip->fire();
			pShot->accessTexture().loadFromFile(pSdlRenderer, "26_motion/shot.bmp");

			// Load game object textures
			if (!pShot->accessTexture().loadFromFile(pSdlRenderer, "26_motion/ship.bmp"))
			{
				printf("Failed to load shot texture!\n");
				success = false;
			}

			pShotList.push_back(pShot);
		}

		pShip->propagate(dt);

		// If the ship went too far left/right
		Vector2D shipPosition = pShip->readPosition();
		double shipRadius = pShip->computeRadius();

		if (shipPosition[0] - shipRadius < 0)
		{
			// Move to the right
			shipPosition[0] = shipRadius;
		}

		if (shipPosition[0] + shipRadius > (double)SCREEN_WIDTH)
		{
			// Move to the left
			shipPosition[0] = SCREEN_WIDTH - (unsigned int) shipRadius;
		}

		// If the ship went too far up/down
		if (shipPosition[1] - shipRadius < 0)
		{
			// Move down
			shipPosition[1] = shipRadius;
		}

		if (shipPosition[1] + shipRadius > (double)SCREEN_HEIGHT)
		{
			// Move up
			shipPosition[1] = SCREEN_HEIGHT - (unsigned int) shipRadius;
		}

		pShip->overridePosition(shipPosition);
		pShip->render(pSdlRenderer);
	}

	try
	{
		// Propagate/Render Comets
		for (std::vector<Comet*>::iterator cItr = pCometList.begin(); cItr != pCometList.end(); ++cItr)
		{
			(*cItr)->propagate(dt);
			(*cItr)->render(pSdlRenderer);
		}

		// Propagate/Render Shots
		for (std::vector<Shot*>::iterator sItr = pShotList.begin(); sItr != pShotList.end(); ++sItr)
		{
			(*sItr)->propagate(dt);
			(*sItr)->render(pSdlRenderer);

			// Remove the shot if it's too old
			if ((*sItr)->readAge() > 3)
			{
				// Destroy shot
				delete* sItr;
				sItr = pShotList.erase(sItr);
			}
		}
	}
	catch (exception ex)
	{
		success = false;
	}

	// Update screen
	SDL_RenderPresent(pSdlRenderer);

	return success;
}




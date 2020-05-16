#pragma once

#include <iostream>
#include <chrono>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "Ship.h"
#include "Shot.h"
#include "Comet.h"

class Game
{
	// SDL vars
	bool readyToPlay = true; // Is game initialized?
	SDL_Window* pSdlWindow = 0; // The window
	SDL_Renderer* pSdlRenderer = 0; // The renderer

	// Game vars
	double sysTime;
	double gameTime;
											              					   
	// Player data vars 
	Ship* pShip; // Player's ship
	bool shipDead; // Is ship dead?
	bool upKeyHeld; // Is accelerating key held?
	bool leftKeyHeld; // Is turn left key held?
	bool rightKeyHeld; // Is turn right key held?
	bool fireKeyHeld; // Is firing key held?
	double timeOfDeath; // Ship time of death
																			   
	// Player associated vars
	std::vector<Shot*> pShotList; // The shots fired by player
																			   
	// Npc data vars 
	std::vector<Comet*> pCometList; // The comets floating around
	
	static Game* pGame; // Game singleton instance
	Game();

	// Functions
	bool createGameObjects();
	bool pollEvents();
	bool handleCollisions();
	bool propagateGameObjects(double);

public:	
	const unsigned int SCREEN_WIDTH = 640; // Screen width
	const unsigned int SCREEN_HEIGHT = 480; // Screen height

	static Game* getInstance();

	// Play game
	void play();
};


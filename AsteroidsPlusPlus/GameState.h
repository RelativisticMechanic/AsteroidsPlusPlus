#pragma once

#define DEFAULT_GAME_LIVES 3

typedef enum
{
	ASTEROID_INTRO_SCREEN,
	ASTEROID_GAME,
	ASTEROID_GAME_OVER
} ASTEROIDGAME_SCREEN;

class GameState
{
public:
	int score;
	int lives;
	int round;
	ASTEROIDGAME_SCREEN screen;

	GameState()
	{
		this->score = 0;
		this->lives = DEFAULT_GAME_LIVES;
		this->round = 0;
		this->screen = ASTEROID_INTRO_SCREEN;
	}

	void Reset()
	{
		this->score = 0;
		this->lives = DEFAULT_GAME_LIVES;
		this->round = 1;
		this->screen = ASTEROID_INTRO_SCREEN;
	}
};
#pragma once
#include "SDL_gpu.h"
#include "SDL_mixer.h"

#include "Timer.h"
#include "Shader.h"
#include "Stars.h"
#include "SpaceShip.h"
#include "Asteroid.h"
#include "Projectile.h"
#include "BitmapFont.h"
#include "GameState.h"
#include "GameHud.h"
#include "Particle.h"
#include <map>

#define ASTEROID_SCORE_MULTIPLIER 10
#define MAX_ASTEROIDS 5
#define BEGIN_TEXT_OFFSET 400.0f
#define N_PARTICLES_ON_ASTEROID_EXPLODE 15

class AsteroidsGame
{
public:
	Stars stars;
	SpaceShip ship;
	std::vector<Asteroid> asteroids;
	std::vector<Projectile> projectiles;
	GameState state;
	GameHud HUD;
	ParticleSystem psystem;

	std::vector<GameEntity> entities;

	float line_thickness = 3.0f;

	Shader retro_shader;
	GPU_Target* screen;
	GPU_Image* internal_buffer;
	GPU_Image* internal_buffer_burn_in;
	GPU_Image* flash;

#ifndef NO_MUSIC_IMPLEMENTATION
	Mix_Chunk* shoot_sound;
	Mix_Chunk* asteroid_sound;
	Mix_Chunk* warn_sound;
	Mix_Chunk* destroyed_sound;
	Mix_Music* song;
	Mix_Music* song_gameover;
#endif

	Timer burn_in_timer;
	Timer cool_down_timer;
	
	/* Used by intro and game over screen */
	float text_offset = BEGIN_TEXT_OFFSET;

	float last_burn_in_time = 0.0f;

	std::map<char, bool> key_states = {
		{ 'W', false },
		{ 'A', false },
		{ 'S', false },
		{ 'D', false },
		{ ' ', false },
		{ '\n', false }
	};

	float waiting_time = 0.0f;

	AsteroidsGame(GPU_Target*);
	void Frame(float delta_time);
	/* Function to draw the game */
	void GameFrame(float delta_time);
	/* Function to draw the intro screen */
	void IntroFrame(float delta_time);
	/* Functio to draw the game over screen */
	void GameOverFrame(float delta_time);
	/* Switch to game over */
	void GameOver();
	/* Switch to Game */
	void BeginGame();
	/* For animating the text offset */
	void AnimateTextOffset(float delta_time);
	/* Make the game wait */
	void WaitingTime(float seconds);

	void HandleEvent(SDL_Event ev, float delta_time);
	void Reset(void);

	
private:
	float resolution_data[2] = { 0.0f, 0.0f };
};
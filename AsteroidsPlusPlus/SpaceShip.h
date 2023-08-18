#pragma once
#include "SDL_gpu.h"
#include "Timer.h"

#include <glm/glm.hpp>

#define SPACESHIP_ROTATION_DECAY_RATE 10.0f
#define SPACESHIP_VELOCITY_DECAY_RATE 0.5f
#define SPACESHIP_MAX_ROTATION_SPEED 250.0f
#define SPACESHIP_ROTATION_ACCEL 5000.0f
#define SPACESHIP_ACCEL 2000.0f

class SpaceShip
{
public:
	glm::vec2 position;
	glm::vec2 look_at;
	float size;
	float radius;

	glm::vec2 velocity;
	float rotation_speed;

	/* Boolean that keeps track whether the ship is blinking */
	bool blinking = false;
	/* Boolean that keeps track whether to show the ship while blinking is ON */
	bool blinking_state = true;

	/* This timer is set when ship is hit */
	Timer toggle_blink_state_timer;
	/* This timer is set when ship undergoes one blink */
	Timer toggle_blink_timer;

	SpaceShip() {};
	SpaceShip(int, int);
	void Frame(GPU_Target*, float);
	void Blink();
};
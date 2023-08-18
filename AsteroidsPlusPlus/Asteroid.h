#pragma once

#include "GameEntity.h"
#include "glm/glm.hpp"
#include "SDL_gpu.h"
#include <vector>

#define ASTEROID_RESOLUTION 16
#define ASTEROID_DEFAULT_RADIUS 128
#define ASTEROID_DEFAULT_DEVIATION 0.2f
#define ASTEROID_MIN_RADIUS 16
#define ASTEROID_STARTING_SPEED 250.0f  
#define ASTEROID_COLOR {255, 255, 255, 255}

class Asteroid : public GameEntity
{
public:
	glm::vec2 screen_bounds;
	std::vector<glm::vec2> render_points;

	Asteroid(glm::vec2 position, glm::vec2 velocity, float radius, glm::vec2 screen_bounds);
	void Update(float delta_time);
	void Draw(GPU_Target*);
	std::vector<Asteroid> Split(glm::vec2 normal);
};
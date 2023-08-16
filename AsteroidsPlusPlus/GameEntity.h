#pragma once

#include "SDL_gpu.h"
#include "glm/glm.hpp"
#include <iostream>

class GameEntity
{
public:
	glm::vec2 position;
	glm::vec2 velocity;
	float radius;
	bool should_remove = false;

	void Update(float delta_time) {};
	void Draw(GPU_Target* t) {};
};
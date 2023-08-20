#pragma once
#include "SDL_gpu.h"
#include "glm/glm.hpp"

#include "GameEntity.h"

#define PROJECTILE_DEFAULT_SPEED 2000.0f
#define PROJECTILE_COOLDOWN_MS 100
#define PROJECTILE_RADIUS 4.0f

class Projectile : public GameEntity
{
public:
	glm::vec2 screen_bounds;

	Projectile() {};
	Projectile(glm::vec2, glm::vec2, glm::vec2);

	void Update(float);
	void Draw(GPU_Target*);
};
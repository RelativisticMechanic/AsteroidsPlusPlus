#include "Projectile.h"
#include <iostream>

Projectile::Projectile(glm::vec2 position, glm::vec2 velocity, glm::vec2 screen_bounds)
{
	this->position = position;
	this->velocity = velocity;
	this->screen_bounds = screen_bounds;
	this->radius = PROJECTILE_RADIUS;
}

void Projectile::Update(float delta_time)
{
	this->position += delta_time * this->velocity;
	if (this->position.x < 0 || this->position.y < 0 || this->position.x >= screen_bounds.x || this->position.y >= screen_bounds.y)
		this->should_remove = true;
}

void Projectile::Draw(GPU_Target* t)
{
	glm::vec2 towards = this->position + glm::normalize(this->velocity) * PROJECTILE_RADIUS * 2.0f;
	GPU_Line(t, this->position.x, this->position.y, towards.x, towards.y, { 255, 255, 255, 255 });
}
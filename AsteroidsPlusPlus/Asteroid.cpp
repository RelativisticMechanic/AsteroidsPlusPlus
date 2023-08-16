#include "Asteroid.h"
#include "Helper.h"

Asteroid::Asteroid(glm::vec2 position, glm::vec2 velocity, float radius, glm::vec2 screen_bounds)
{
	this->position = position;
	this->velocity = velocity;
	this->radius = radius;
		
	/* Now create points */
	this->render_points = std::vector<glm::vec2>(ASTEROID_RESOLUTION, glm::vec2(0.0f, 0.0f));

	/* Iterate from 0 to 2pi */
	for (int i = 0; i < ASTEROID_RESOLUTION; i++)
	{
		float angle = i * (2 * CONSTANT_PI / ASTEROID_RESOLUTION);
		float randomized_radius = radius * (1.0f + RandRange(-1.0f, 1.0f) * ASTEROID_DEFAULT_DEVIATION);
		this->render_points[i] = randomized_radius * glm::vec2(cosf(angle), sinf(angle));
	}

	this->screen_bounds = screen_bounds;
}

void Asteroid::Update(float delta_time)
{
	this->position += delta_time * this->velocity;

	if (this->position.x < -this->radius)
	{
		this->position.x = screen_bounds.x + this->radius;
	}
	if (this->position.x > screen_bounds.x + this->radius)
	{
		this->position.x = -this->radius;
	}
	if (this->position.y < -this->radius)
	{
		this->position.y = screen_bounds.y + this->radius;
	}
	if (this->position.y > screen_bounds.y + this->radius)
	{
		this->position.y = -this->radius;
	}
}

void Asteroid::Draw(GPU_Target* t)
{
	for (int i = 0; i < ASTEROID_RESOLUTION - 1; i++)
	{
		glm::vec2 p1 = this->position + render_points[i];
		glm::vec2 p2 = this->position + render_points[i+1];
		GPU_Line(t, p1.x, p1.y, p2.x, p2.y, { 255, 255, 255, 255 });
	}

	glm::vec2 p1 = this->position + render_points[0];
	glm::vec2 pf = this->position + render_points[ASTEROID_RESOLUTION - 1];
	GPU_Line(t, p1.x, p1.y, pf.x, pf.y, { 255, 255, 255, 255 });
}

std::vector<Asteroid> Asteroid::Split(glm::vec2 velocity_dir)
{

	glm::vec2 normal = glm::normalize(glm::vec2(-velocity_dir.y, velocity_dir.x));
	std::vector<Asteroid> result; 
	this->should_remove = true; 

	float new_radius = this->radius / 2.0f;

	if (new_radius < ASTEROID_MIN_RADIUS)
		return result;

	float speed = glm::length(this->velocity);

	result.push_back(Asteroid(this->position + (this->radius / 2.0f) * normal, speed * normal * 1.5f, (this->radius / 2.0f), screen_bounds));
	result.push_back(Asteroid(this->position - (this->radius / 2.0f) * normal, speed * normal * (-1.5f), (this->radius / 2.0f), screen_bounds));

	return result;
}
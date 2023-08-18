#pragma once

#include "glm/glm.hpp"
#include "Shader.h"
#include <vector>
#include <iostream>

class Particle
{
public:
	glm::vec2 position;
	glm::vec2 velocity;
	float size = 1.0f;
	float exists = true;

	Particle(glm::vec2 p, glm::vec2 v, float sz) : position(p), velocity(v), size(sz) {};
	void Destroy()
	{
		this->exists = false;
	}
};

class ParticleSystem
{
public:
	std::vector<Particle> particles;
	Shader particle_shader;
	glm::vec2 bounds;

	ParticleSystem() {};
	ParticleSystem(glm::vec2 bounds, std::string shader);
	void Add(Particle p);

	void ParticleFunc(Particle& p, float delta_time) 
	{
		p.position += p.velocity * delta_time;
	};

	void Update(float delta_time);
	void Draw(GPU_Target* t);
	void Clear(void);
};
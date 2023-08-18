#include "Particle.h"

ParticleSystem::ParticleSystem(glm::vec2 bounds, std::string shader_path)
{
	this->particle_shader = Shader(shader_path);
	this->bounds = bounds;
}

void ParticleSystem::Update(float delta_time)
{
	for (std::vector<Particle>::iterator particle_iterator = particles.begin(); particle_iterator != particles.end(); /* */)
	{
		Particle& particle = *particle_iterator;
		this->ParticleFunc(particle, delta_time);
		/* Check if particle is outside bounds */
		if (particle.position.x < 0 || particle.position.y < 0 || particle.position.x >= bounds.x || particle.position.y >= bounds.y)
		{
			particle.Destroy();
		}

		/* Delete the particle if exists is false */
		if (!particle.exists)
		{
			particle_iterator = particles.erase(particle_iterator);
		}
		else
		{
			particle_iterator++;
		}
	}
}

void ParticleSystem::Add(Particle p)
{
	this->particles.push_back(p);
}

void ParticleSystem::Clear(void)
{
	this->particles.clear();
}

void ParticleSystem::Draw(GPU_Target* t)
{
	this->particle_shader.ActivateShader();
	float time = SDL_GetTicks64() / 1000.0f;
	GPU_SetUniformf(particle_shader.UniformLocation("time"), time);
	for (auto& particle : particles)
	{
		float x1 = particle.position.x - (particle.size / 2.0f);
		float x2 = particle.position.x + (particle.size / 2.0f);
		float y1 = particle.position.y - (particle.size / 2.0f);
		float y2 = particle.position.y + (particle.size / 2.0f);
		GPU_SetUniformf(particle_shader.UniformLocation("particle_size"), particle.size);
		GPU_RectangleFilled(t, x1, y1, x2, y2, { 255, 255, 255, 255 });
	}
}
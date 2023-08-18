#include "Game.h"
#include "Helper.h"

#include <iostream>

AsteroidsGame::AsteroidsGame(GPU_Target* screen)
{
	srand(2039203112);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	this->screen = screen;
	/* This is the buffer where all the frames are drawn */
	this->internal_buffer = GPU_CreateImage(screen->w, screen->h, GPU_FORMAT_RGBA);
	/* This one is for the burn in effect */
	this->internal_buffer_burn_in = GPU_CreateImage(screen->w, screen->h, GPU_FORMAT_RGBA);
	/* This one is for the flash */
	this->flash = GPU_CreateImage(screen->w, screen->h, GPU_FORMAT_RGBA);
	GPU_LoadTarget(this->internal_buffer);
	GPU_LoadTarget(this->internal_buffer_burn_in);
	GPU_LoadTarget(this->flash);

	GPU_ClearRGBA(this->flash->target, 255, 0, 0, 64);

	this->retro_shader = Shader("shaders/retro");
	this->stars = Stars(screen->w, screen->h, 3, 0.0015);;
	this->ship = SpaceShip(screen->w, screen->h);
	this->psystem = ParticleSystem(glm::vec2(screen->w, screen->h), "shaders/particle");

	this->burn_in_timer = Timer(2000);
	this->cool_down_timer = Timer(PROJECTILE_COOLDOWN_MS);


	resolution_data[0] = screen->w;
	resolution_data[1] = screen->h;

	this->shoot_sound = Mix_LoadWAV("sound/shoot.wav");
	this->asteroid_sound = Mix_LoadWAV("sound/asteroid.wav");
	this->warn_sound = Mix_LoadWAV("sound/warn.wav");
	this->song = Mix_LoadMUS("sound/asteroids.mp3");
	this->song_gameover = Mix_LoadMUS("sound/over.mp3");
	
	GPU_SetLineThickness(this->line_thickness);
}

void AsteroidsGame::IntroFrame(float delta_time)
{
	if (key_states['\n'])
	{
		this->BeginGame();
	}

	this->AnimateTextOffset(delta_time);
	this->psystem.Update(delta_time);

	GPU_DeactivateShaderProgram();
	
	HUD.font.DrawText(this->internal_buffer->target, this->internal_buffer->w * 0.33, this->internal_buffer->h * 0.35 + this->text_offset, 4.0f, "ASTEROIDS++");
	HUD.font.DrawText(this->internal_buffer->target, this->internal_buffer->w * 0.33, this->internal_buffer->h * 0.45 + this->text_offset, 2.0f, "A game by Siddharth Gautam");
	HUD.font.DrawText(this->internal_buffer->target, this->internal_buffer->w * 0.33, this->internal_buffer->h * 0.55 + this->text_offset, 2.0f, "Press <RETURN> to begin");
	
	this->psystem.Draw(this->internal_buffer->target);
}

void AsteroidsGame::GameOverFrame(float delta_time)
{
	if (key_states['\n'])
	{
		this->BeginGame();
	}
	
	this->AnimateTextOffset(delta_time);

	GPU_DeactivateShaderProgram();
	HUD.font.DrawText(this->internal_buffer->target, this->internal_buffer->w * 0.42f, this->internal_buffer->h * 0.35 + this->text_offset, 5.0f, "GAME");
	HUD.font.DrawText(this->internal_buffer->target, this->internal_buffer->w * 0.42f, this->internal_buffer->h * 0.45 + this->text_offset, 5.0f, "OVER!");
	HUD.font.DrawText(this->internal_buffer->target, this->internal_buffer->w * 0.20f, this->internal_buffer->h * 0.55 + this->text_offset, 2.0f, "Your spaceship was casted to the void* of samsara.");
	HUD.font.DrawText(this->internal_buffer->target, this->internal_buffer->w * 0.33f, this->internal_buffer->h * 0.65 + this->text_offset, 2.0f, "Press <RETURN> to go back");
}

void AsteroidsGame::GameFrame(float delta_time)
{
	if (state.lives < 0)
	{
		this->GameOver();
	}

	if (key_states['A'])
		this->ship.rotation_speed += Clamp(SPACESHIP_ROTATION_ACCEL * delta_time, 0.0f, SPACESHIP_MAX_ROTATION_SPEED);

	if (key_states['D'])
		this->ship.rotation_speed -= Clamp(SPACESHIP_ROTATION_ACCEL * delta_time, 0.0f, SPACESHIP_MAX_ROTATION_SPEED);

	if (key_states['W'])
	{
		this->ship.velocity += SPACESHIP_ACCEL * delta_time * this->ship.look_at;
		glm::vec2 new_velocity = -this->ship.look_at * 400.0f * RandRange(0.9, 1.0);
		new_velocity += RandVec2(25.0f);
		this->psystem.Add(Particle(this->ship.position, new_velocity, 2.0f));
	}

	if (key_states['S'])
		this->ship.velocity -= SPACESHIP_ACCEL * delta_time * this->ship.look_at;

	if (key_states[' '] && this->cool_down_timer.Elapsed())
	{
		Mix_PlayChannel(-1, this->shoot_sound, 0);
		this->projectiles.push_back(Projectile(this->ship.position, PROJECTILE_DEFAULT_SPEED * this->ship.look_at, glm::vec2(screen->w, screen->h)));
		this->cool_down_timer.Reset();
	}

	this->psystem.Update(delta_time);
	/* Update Projectiles */
	for (std::vector<Projectile>::iterator projectile = projectiles.begin(); projectile != projectiles.end(); /* */)
	{
		(*projectile).Update(delta_time);

		/* If out of bounds, remove */
		if ((*projectile).should_remove)
		{
			/* stdc++ = erase points to next iterator, neat. */
			projectile = projectiles.erase(projectile);
		}
		else
		{
			projectile++;
		}
	}

	/* Vector to store new asteroids created as a result of split */
	std::vector<Asteroid> new_asteroids;

	/* Update Asteroids */
	for (std::vector<Asteroid>::iterator asteroid = asteroids.begin(); asteroid != asteroids.end(); /* */)
	{
		(*asteroid).Update(delta_time);
		/* Check if we are colliding with player */
		if (glm::length((*asteroid).position - ship.position) < (*asteroid).radius + ship.radius && !ship.blinking)
		{	
			ship.Blink();
			state.lives -= 1;
			if (state.lives >= 0)
			{
				Mix_PlayChannel(-1, this->warn_sound, 1);
			}
		}
	
		// ENABLE THIS SECTION OF CODE IF YOU WANT COLLIDING ASTEROIDS
		// ITS KINDA COOL BUT NOT FAITHFUL TO THE ORIGINAL GAME

		// Check if asteroid is colliding with other asteroids
		for (std::vector<Asteroid>::iterator other = asteroids.begin(); other != asteroids.end(); )
		{
			float distance = glm::length((*asteroid).position - (*other).position);
			float min_collision_dist = (*asteroid).radius + (*other).radius;
			if (distance <= min_collision_dist && other != asteroid)
			{
				// We simulate an elastic collision where the component of velocity
				// perpendicular to the collision normal is inversed.
				glm::vec2 normal_direction = glm::normalize((*other).position - (*asteroid).position);

				// Dot product tells me the component of velocity in the direction of 
				glm::vec2 normal_velocity = glm::dot((*asteroid).velocity, normal_direction) * normal_direction;
				glm::vec2 other_normal_velocity = glm::dot((*other).velocity, -normal_direction) * (-normal_direction);

				// Inverse the component of normal velocities
				(*asteroid).velocity -= 2.0f * normal_velocity;
				(*other).velocity -= 2.0f * other_normal_velocity;

				// Push the asteroids back to a safe distance (this prevents multiple collisions from happening)
				float push_back = (min_collision_dist - distance) * 0.5f;

				(*asteroid).position -= normal_direction * push_back;
				(*other).position += normal_direction * push_back;
			}

			other++;
		}
		

		/* Check if this asteroid is colliding with any projectile */
		for (auto& projectile : projectiles)
		{
			if (glm::length((*asteroid).position - projectile.position) < ((*asteroid).radius + projectile.radius))
			{
				/* Add some particles */
				for (int i = 0; i < N_PARTICLES_ON_ASTEROID_EXPLODE; i++)
				{
					this->psystem.Add(Particle(projectile.position, RandVec2(200.0f), 4.0f));
				}

				/* Play asteroid sound */
				Mix_PlayChannel(-1, this->asteroid_sound, 0);
				std::vector<Asteroid> two_new_asteroids = (*asteroid).Split(projectile.velocity);
				for (int i = 0; i < two_new_asteroids.size(); i++)
				{
					new_asteroids.push_back(two_new_asteroids[i]);
				}
				projectile.should_remove = true;

				state.score += ASTEROID_SCORE_MULTIPLIER * (ASTEROID_DEFAULT_RADIUS / (*asteroid).radius);
			}
		}

		/* If asteroid is queued to remove, remove it */
		if ((*asteroid).should_remove)
		{
			/* stdc++ = erase points to next iterator, neat. */
			asteroid = asteroids.erase(asteroid);
		}
		else
		{
			asteroid++;
		}
	}

	/* Add the new asteroids */
	for (auto new_asteroid : new_asteroids)
	{
		asteroids.push_back(new_asteroid);
	}

	if (this->asteroids.size() == 0)
	{
		/* Next round */
		state.round++;
		
		int n_asteroids_to_generate = ((rand() % state.round) % MAX_ASTEROIDS) + 1;
		for (int i = 0; i < n_asteroids_to_generate; i++)
		{
			this->asteroids.push_back(Asteroid(glm::vec2(rand() % screen->w, -ASTEROID_DEFAULT_RADIUS), RandVec2(ASTEROID_STARTING_SPEED), ASTEROID_DEFAULT_RADIUS, glm::vec2(screen->w, screen->h)));
		}
	}

	/* Draw Stars */
	this->stars.Draw(this->internal_buffer->target);

	if (ship.blinking && !ship.blinking_state)
	{
		GPU_Blit(this->flash, NULL, this->internal_buffer->target, this->internal_buffer->w / 2, this->internal_buffer->h / 2);
		HUD.font.DrawText(this->internal_buffer->target, screen->w * 0.28, screen->h * 0.35, 8.0f, "HIT TAKEN!", { 255, 255, 0, 255 });
	}

	/* Update and Draw Ship */
	this->ship.Frame(this->internal_buffer->target, delta_time);

	/* Draw Projectiles */
	for (auto& projectile : projectiles)
	{
		projectile.Draw(this->internal_buffer->target);
	}

	for (auto& asteroid : asteroids)
	{
		asteroid.Draw(this->internal_buffer->target);
	}

	this->psystem.Draw(this->internal_buffer->target);


	HUD.Draw(this->internal_buffer->target, state);
}
void AsteroidsGame::Frame(float delta_time)
{
	GPU_DeactivateShaderProgram();
	/* Check if time has passed to clean up the burn in buffer */
	if (this->burn_in_timer.Elapsed())
	{
		GPU_Clear(this->internal_buffer_burn_in->target);
		burn_in_timer.Reset();
		last_burn_in_time = SDL_GetTicks64();
	}
	/* Blit the last frame onto the burn in source */
	GPU_Blit(this->internal_buffer, NULL, this->internal_buffer_burn_in->target, this->internal_buffer_burn_in->w / 2, this->internal_buffer_burn_in->h / 2);
	GPU_Clear(this->internal_buffer->target);

	switch (this->state.screen)
	{
	case ASTEROID_GAME:
		this->GameFrame(delta_time);
		break;
	case ASTEROID_INTRO_SCREEN:
		this->IntroFrame(delta_time);
		break;
	case ASTEROID_GAME_OVER:
		this->GameOverFrame(delta_time);
		break;
	default:
		break;
	}
	
	/* Post-Processing */
	this->retro_shader.ActivateShader();
	uint64_t time = SDL_GetTicks64();
	GPU_SetUniformf(retro_shader.UniformLocation("time"), (float)time / 1000.0f);
	GPU_SetUniformf(retro_shader.UniformLocation("last_burn"), this->last_burn_in_time);
	GPU_SetUniformf(retro_shader.UniformLocation("current_burn"), time);
	GPU_SetUniformfv(retro_shader.UniformLocation("resolution"), 2, 1, (float*)&resolution_data);
	GPU_SetShaderImage(this->internal_buffer_burn_in, retro_shader.UniformLocation("burn_in_source"), 2);
	GPU_Blit(this->internal_buffer, NULL, this->screen, this->screen->w / 2, this->screen->h / 2);
}

void AsteroidsGame::Reset(void)
{
	/* Reset ship */
	this->ship = SpaceShip(screen->w, screen->h);
	/* Clear Asteroids */
	this->asteroids.clear();
	/* Clear projectiles */
	this->projectiles.clear();
	/* Clear particles */
	this->psystem.Clear();
	/* Reset score and lives */
	this->state.Reset();
}

void AsteroidsGame::GameOver()
{
	this->Reset();
	this->state.screen = ASTEROID_GAME_OVER;
	this->text_offset = BEGIN_TEXT_OFFSET;
	Mix_HaltMusic();
	Mix_PlayMusic(this->song_gameover, -1);
}

void AsteroidsGame::BeginGame()
{
	this->state.screen = ASTEROID_GAME;
	Mix_HaltMusic();
	Mix_PlayMusic(this->song, -1);
}

void AsteroidsGame::AnimateTextOffset(float delta_time)
{
	if (this->text_offset > 0.0f)
	{
		this->text_offset -= 20.0f * delta_time;
	}
	else
	{
		this->text_offset = 0.0f;
	}
}

void AsteroidsGame::HandleEvent(SDL_Event ev, float delta_time)
{
	switch (ev.type)
	{
	case SDL_KEYDOWN:
		switch (ev.key.keysym.sym)
		{
		case SDLK_a:
		case SDLK_LEFT:
			key_states['A'] = true;
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			key_states['D'] = true;
			break;
		case SDLK_w:
		case SDLK_UP:
			key_states['W'] = true;
			break;
		case SDLK_s:
		case SDLK_DOWN:
			key_states['S'] = true;
			break; 
		case SDLK_SPACE:
			key_states[' '] = true;
			break;
		case SDLK_KP_ENTER:
		case SDLK_RETURN:
		case SDLK_RETURN2:
			key_states['\n'] = true;
			break;
		default:
			break;
		}
		break;

	case SDL_KEYUP:
		switch (ev.key.keysym.sym)
		{
		case SDLK_a:
		case SDLK_LEFT:
			key_states['A'] = false;
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			key_states['D'] = false;
			break;
		case SDLK_w:
		case SDLK_UP:
			key_states['W'] = false;
			break;
		case SDLK_s:
		case SDLK_DOWN:
			key_states['S'] = false;
			break;
		case SDLK_SPACE:
			key_states[' '] = false;
			break;
		case SDLK_KP_ENTER:
		case SDLK_RETURN:
		case SDLK_RETURN2:
			key_states['\n'] = false;
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}
}


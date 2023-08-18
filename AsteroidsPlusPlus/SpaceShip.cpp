#include "SpaceShip.h"
#include "Helper.h"

#include <iostream>
#include <cmath>

SpaceShip::SpaceShip(int SCREEN_W, int SCREEN_H)
{
	this->position = glm::vec2(SCREEN_W / 2, SCREEN_H / 2);
	this->look_at = glm::vec2(1.0, 0.0);
	this->size = 30.0;
	this->rotation_speed = 0.0f;
	this->velocity = glm::vec2(0.0f, 0.0f);
	this->toggle_blink_timer = Timer(SPACESHIP_HIT_COOLDOWN_TIME_MS);
	this->toggle_blink_state_timer = Timer(100.0f);
}

void SpaceShip::Blink()
{
	this->toggle_blink_timer.Reset();
	this->toggle_blink_state_timer.Reset();
	this->blinking = true;
	this->blinking_state = false;
	this->radius = (sqrtf(15.0f) * this->size / 3.0f) * 0.5f;

}

void SpaceShip::Frame(GPU_Target* t, float delta_time)
{
	/* Do rotation */
	if (fabs(rotation_speed) > 0.1f)
	{
		/* Lerp to 0 */
		rotation_speed = Decay(rotation_speed, delta_time * SPACESHIP_ROTATION_DECAY_RATE);
		/* Rotate */
		this->look_at = Rotate2D(this->look_at, rotation_speed * delta_time);
	}
	else
	{
		rotation_speed = 0.0f;
	}

	if (this->velocity.length() > 0.01f)
	{
		this->position += delta_time * this->velocity;
		this->velocity -= SPACESHIP_VELOCITY_DECAY_RATE * delta_time * this->velocity;
	}
	else
	{
		this->velocity = glm::vec2(0.0f, 0.0f);
	}

	/* Limit screen space */
	if (this->position.x < 0)
	{
		this->position.x = t->w;
	}
	if (this->position.y < 0)
	{
		this->position.y = t->h;
	}
	if (this->position.x > t->w)
	{
		this->position.x = 0;
	}
	if (this->position.y > t->h)
	{
		this->position.y = 0;
	}
	/* Calculate coordinates */
	glm::vec2 v1 = (sqrtf(15.0f) * this->size / 3.0f) * this->look_at;

	/* Rotate v1 by 120 and -120 to get the other two points. */
	glm::vec2 v2 = 0.5f * Rotate2D(v1, 120.0f);
	glm::vec2 v3 = 0.5f * Rotate2D(v1, -120.0f);

	v1 += this->position;
	v2 += this->position;
	v3 += this->position;

	/* Are we blinking? */
	if (this->blinking)
	{
		if (this->toggle_blink_timer.Elapsed())
		{
			this->blinking = false;
			this->blinking_state = false;
		}

		if (this->toggle_blink_state_timer.Elapsed())
		{
			this->blinking_state = !this->blinking_state;
			this->toggle_blink_state_timer.Reset();
		}

		if (this->blinking_state)
		{
			GPU_Line(t, v1.x, v1.y, v2.x, v2.y, SPACESHIP_FLASHING_COLOR);
			GPU_Line(t, v1.x, v1.y, v3.x, v3.y, SPACESHIP_FLASHING_COLOR);
			GPU_Line(t, v2.x, v2.y, this->position.x, this->position.y, SPACESHIP_FLASHING_COLOR);
			GPU_Line(t, v3.x, v3.y, this->position.x, this->position.y, SPACESHIP_FLASHING_COLOR);

		}
	}
	else
	{
		GPU_Line(t, v1.x, v1.y, v2.x, v2.y, SPACESHIP_DEFAULT_COLOR);
		GPU_Line(t, v1.x, v1.y, v3.x, v3.y, SPACESHIP_DEFAULT_COLOR);
		GPU_Line(t, v2.x, v2.y, this->position.x, this->position.y, SPACESHIP_DEFAULT_COLOR);
		GPU_Line(t, v3.x, v3.y, this->position.x, this->position.y, SPACESHIP_DEFAULT_COLOR);
	}
}
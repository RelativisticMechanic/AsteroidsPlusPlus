#pragma once

#include "SDL_gpu.h"
#include <cstdlib>

class Stars
{
public:
	float scale = 0.0f;
	GPU_Image* stars_background;

	Stars() {};
	Stars(int screen_w, int screen_h, int star_size, float percent)
	{
		this->stars_background = GPU_CreateImage(screen_w / star_size, screen_h / star_size, GPU_FORMAT_RGBA);
		GPU_LoadTarget(this->stars_background);

		this->scale = star_size;
		int max_width = screen_w / star_size;
		int max_height = screen_h / star_size;
		int n = max_width * max_height * percent;

		for (int i = 0; i < n; i++)
		{
			int x = rand() % max_width;
			int y = rand() % max_height;

			GPU_Pixel(this->stars_background->target, x, y, { 255, 255, 255, 255 });
		}
	}

	void Draw(GPU_Target* screen)
	{
		GPU_BlitScale(this->stars_background, NULL, screen, screen->w / 2, screen->h / 2, scale, scale);
	}
};
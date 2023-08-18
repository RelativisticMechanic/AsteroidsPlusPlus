#pragma once

#include "SDL_gpu.h"
#include <cstdlib>
#include <iostream>
class Stars
{
public:
	float scale = 0.0f;
	GPU_Image* stars_background;

	Stars() {};
	Stars(int screen_w, int screen_h, int star_size, float percent)
	{
		this->stars_background = GPU_CreateImage(screen_w, screen_h, GPU_FORMAT_RGBA);
		GPU_LoadTarget(this->stars_background);

		int n = screen_w * screen_h * percent;

		for (int i = 0; i < n; i++)
		{
			int x = ((float)rand() / (float)RAND_MAX) * screen_w;
			int y = ((float)rand() / (float)RAND_MAX) * screen_h;
			GPU_RectangleFilled(this->stars_background->target, x, y, (x+star_size), y+star_size, { 255, 255, 255, 255 });
		}
	}

	void Draw(GPU_Target* screen)
	{
		GPU_Blit(this->stars_background, NULL, screen, screen->w / 2, screen->h / 2);
	}
};
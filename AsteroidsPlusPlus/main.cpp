#include "SDL_gpu.h"
#include "Game.h"
#include "Stars.h"
#include "Shader.h"

#define SCREEN_W 1280 
#define SCREEN_H 720

int main(int argc, char* argv[])
{
	/* Create screen and get window */
	GPU_SetDebugLevel(GPU_DEBUG_LEVEL_MAX);
	GPU_Target* screen = GPU_Init(SCREEN_W, SCREEN_H, GPU_DEFAULT_INIT_FLAGS);
	SDL_Window* window = SDL_GetWindowFromID(screen->context->windowID);
	if (!screen || !window)
		return -1;

	float resolution_data[2] = { SCREEN_W, SCREEN_H };

	/* Set Window Title */
	SDL_SetWindowTitle(window, "Asteroids++");
	AsteroidsGame game = AsteroidsGame(screen);

	/* Event loop */
	bool running = true;
	uint64_t previous_time = 0;
	uint64_t current_time = SDL_GetTicks64();
	float delta_time = 0.0f;

	while (running)
	{
		delta_time = (current_time - previous_time) / 1000.0f;
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			default:
				break;
			}
			game.HandleEvent(ev, delta_time);
		}

		/* Clear and update screen every frame */
		GPU_ClearColor(screen, { 0, 0, 0, 0 });
		for (int i = 0; i < 5; i++)
		{
			game.Frame(delta_time / 5.0f);
		}
		//game.Frame(delta_time);
		GPU_Flip(screen);
		previous_time = current_time;
		current_time = SDL_GetTicks64();
	}

	return 0;
}
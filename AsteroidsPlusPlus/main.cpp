#include "SDL_gpu.h"
#include "Game.h"
#include "Stars.h"
#include "Shader.h"

#ifdef EMSCRIPTEN_IMPLEMENTATION
#include <emscripten.h>
#endif

GPU_Target* screen = NULL;

void MainLoop(void)
{
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
}
int main(int argc, char* argv[])
{
	/* Create screen and get window */
	GPU_SetDebugLevel(GPU_DEBUG_LEVEL_MAX);
	int screen_w = 1280;
	int screen_h = 720;

	screen = GPU_Init(screen_w, screen_h, GPU_DEFAULT_INIT_FLAGS);

	/* Open audio */
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	SDL_Window* window = SDL_GetWindowFromID(screen->context->windowID);
	if (!screen || !window)
		return -1;

	float resolution_data[2] = { screen->w, screen->h };

	/* Set Window Title */
	SDL_SetWindowTitle(window, "Asteroids++");
	
#ifdef ENGINE2D_EMSCRIPTEN_IMPLEMENTATION
	emscripten_set_main_loop(MainLoop, 0, 1);
#else
	MainLoop();
#endif

	return 0;
}
#pragma once

#include "SDL_gpu.h"
#include <string>

#include "GameState.h"
#include "BitmapFont.h"

class GameHud
{
public:
	BitmapFont font;

	GameHud() {};
	void Draw(GPU_Target* t, GameState state);
};
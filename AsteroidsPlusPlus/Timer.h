#pragma once

#include "SDL.h"
#include <cstdint>

class Timer
{
public:
	uint64_t previous;
	uint64_t interval_ms;

	Timer() {};

	Timer(uint64_t interval)
	{
		this->interval_ms = interval; 
		this->previous = SDL_GetTicks64();
	}

	bool Elapsed()
	{
		if (SDL_GetTicks64() - interval_ms >= previous)
		{
			return true;
		}
		return false;
	}

	void Reset()
	{
		this->previous = SDL_GetTicks64();
	}

};
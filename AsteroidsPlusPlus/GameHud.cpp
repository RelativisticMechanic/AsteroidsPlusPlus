#include "GameHud.h"


void GameHud::Draw(GPU_Target* t, GameState state)
{
	/* Draw score */
	std::string score_text = "SCORE: " + std::to_string(state.score) + " LIVES: " + std::to_string(state.lives) + " ROUND: " + std::to_string(state.round);
	font.DrawText(t, t->w * 0.27, t->h - font.GetYAdvance() * 3.0, 3.0f, score_text.c_str());
}
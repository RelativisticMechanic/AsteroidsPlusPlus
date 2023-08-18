#include "GameHud.h"


void GameHud::Draw(GPU_Target* t, GameState state)
{
	GPU_DeactivateShaderProgram();
	/* Draw score */
	std::string score_text = "SCORE: " + std::to_string(state.score) + " ARMOR: " + std::to_string((int)((((float)state.lives) / (float)DEFAULT_GAME_LIVES) * 100.0f)) + "% ROUND: " + std::to_string(state.round);
	font.DrawText(t, t->w * 0.2, t->h - font.GetYAdvance() * 3.0, 3.0f, score_text.c_str());
}
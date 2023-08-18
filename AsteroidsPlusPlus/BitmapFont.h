#pragma once
#include "SDL_gpu.h"
#include "Shader.h"

#include <cstdint>
#include <string>

extern uint8_t VGA437_data[32768];
extern int VGA437_char_width;
extern int VGA437_char_height;
extern int VGA437_font_bitmap_width;
extern int VGA437_font_bitmap_height;

class BitmapFont
{
public:
	BitmapFont();
	~BitmapFont();

	GPU_Image* GetGlyph(unsigned char);
	int GetXAdvance();
	int GetYAdvance();

	void DrawText(GPU_Target* t, int x, int y, float scale, const char* txt, SDL_Color color = { 0, 128, 0, 255 });

private:
	GPU_Image* characters[256];
	GPU_Image* font_bitmap;
	int char_width, char_height;
	Shader text_shader;
};
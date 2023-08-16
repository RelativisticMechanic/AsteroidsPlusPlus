#include "BitmapFont.h"
#include "SDL.h"
#include <iostream>
#include <cassert>

BitmapFont::BitmapFont(void)
{
	SDL_Surface* font_surface = SDL_CreateRGBSurfaceFrom((void*)&VGA437_data, VGA437_font_bitmap_width, VGA437_font_bitmap_height, 8, VGA437_font_bitmap_width, 0, 0, 0, 1);
	assert(font_surface != NULL);

	/* Set the palette colors accordingly 0 = black, 255 = white */
	SDL_Color colors[2];
	colors[0] = { 0, 0, 0, 0 };
	colors[1] = { 0, 128, 0, 255 };

	SDL_SetPaletteColors(font_surface->format->palette, colors, 0, 2);
	/* Make black transparent */
	SDL_SetColorKey(font_surface, SDL_TRUE, 0);

	this->char_width = VGA437_char_width;
	this->char_height = VGA437_char_height;

	this->font_bitmap = GPU_CopyImageFromSurface(font_surface);

	int chars_per_row = font_bitmap->w / this->char_width;
	for (int i = 0; i < 256; i++)
	{
		GPU_Rect r;
		r.x = (i % chars_per_row) * this->char_width;
		r.y = (i / chars_per_row) * this->char_height;
		r.w = this->char_width;
		r.h = this->char_height;
		this->characters[i] = GPU_CreateImage(this->char_width, this->char_height, GPU_FORMAT_RGBA);
		GPU_Target* tg = GPU_LoadTarget(this->characters[i]);
		if (tg != this->characters[i]->target || tg == NULL)
		{
			std::cerr << "Warning: GPU_LoadTarget Failed!" << std::endl;
			exit(-1);
		}
		GPU_Blit(font_bitmap, &r, tg, this->char_width / 2.0, this->char_height / 2.0);
	}
}

int BitmapFont::GetXAdvance()
{
	return this->char_width;
}

int BitmapFont::GetYAdvance()
{
	return this->char_height;
}

GPU_Image* BitmapFont::GetGlyph(unsigned char c)
{
	return this->characters[c];
}

void BitmapFont::DrawText(GPU_Target* t, int x, int y, float scale, const char* txt)
{
	int n = strlen(txt);
	int curr_x = x;
	int curr_y = y;
	for (int i = 0; i < n; i++)
	{
		unsigned char c = txt[i];
		GPU_Image* glyph = this->GetGlyph(c);
		GPU_BlitScale(glyph, NULL, t, curr_x + scale * (this->char_width) / 2, curr_y + scale * this->char_height / 2, scale, scale);
		curr_x += scale * this->char_width;
	}
}

BitmapFont::~BitmapFont()
{
	for (int i = 0; i < 256; i++)
	{
		GPU_FreeImage(this->characters[i]);
	}
	GPU_FreeImage(this->font_bitmap);
}
#pragma once
#include <string>
#include <cstdint>

#include "SDL_gpu.h"


class Shader
{
public:
	uint32_t program_id;
	GPU_ShaderBlock shader_block;

	Shader() {};
	Shader(std::string name);
	void ActivateShader();
	uint32_t UniformLocation(const char* uniform);
};
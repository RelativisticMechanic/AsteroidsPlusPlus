#include "Shader.h"

uint32_t loadShader(GPU_ShaderEnum type, const char* filename)
{
	SDL_RWops* rwops;
	uint32_t shader;
	int file_size;

	rwops = SDL_RWFromFile(filename, "rb");
	if (rwops == NULL)
	{
		GPU_PushErrorCode("loadShader", GPU_ERROR_FILE_NOT_FOUND, "Shader file \"%s\" not found", filename);
		return 0;
	}
	/* Get size of shader file */
	file_size = SDL_RWseek(rwops, 0, SEEK_END);
	/* Reset back to beginning */
	SDL_RWseek(rwops, 0, SEEK_SET);

	/* Load the source */
	char* source = (char*)malloc((file_size + 1 /* 1 byte for '\0' */) * sizeof(char));
	SDL_RWread(rwops, source, sizeof(char), file_size);
	source[file_size] = '\0';
	
	std::string source_shader_with_version = std::string(source);
#ifdef EMSCRIPTEN_IMPLEMENTATION
	source_shader_with_version = "#version 300 es\nprecision mediump float;\n" + source_shader_with_version;
#else
	source_shader_with_version = "#version 330\n" + source_shader_with_version;
#endif

	/* Compile the shader */
	shader = GPU_CompileShader(type, source_shader_with_version.c_str());

	/* Free resources */
	free(source);
	SDL_RWclose(rwops);

	return shader;
}

Shader::Shader(std::string name)
{
	/* Load the vertex and fragment shaders */
	std::string vsname = name + ".vs.glsl";
	std::string fsname = name + ".fs.glsl";

	uint32_t v = loadShader(GPU_VERTEX_SHADER, vsname.c_str());
	uint32_t f = loadShader(GPU_PIXEL_SHADER, fsname.c_str());

	if (!v)
		GPU_LogError("Unable to load vertex shader: %s, error:\n%s\n", vsname.c_str(), GPU_GetShaderMessage());
	if (!f)
		GPU_LogError("Unable to load fragment shader: %s, error:\n%s\n", fsname.c_str(), GPU_GetShaderMessage());

	this->program_id = GPU_LinkShaders(v, f);
	if (!(this->program_id))
	{
		GPU_LogError("Unable to link shaders [%s] [%s], error:\n%s\n", vsname.c_str(), fsname.c_str(), GPU_GetShaderMessage());
		this->shader_block = { -1, -1, -1, -1 };
	}
	this->shader_block = GPU_LoadShaderBlock(this->program_id, "gpu_Vertex", "gpu_TexCoord", "gpu_Color", "gpu_ModelViewProjectionMatrix");
}
 
void Shader::ActivateShader()
{
	GPU_ActivateShaderProgram(this->program_id, &this->shader_block);
}

uint32_t Shader::UniformLocation(const char* uniform)
{
	return GPU_GetUniformLocation(this->program_id, uniform);
}

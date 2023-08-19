# Compiling SDL2-GPU apps for the Web with Emscripten

So, since it seems that no documentation was available about this, I decided to explore on my own how to get this game running on the web with Emscripten.

Emscripten is a LLVM-based compiler that turns C/C++ code into WASM (Web Assembly), that means your C/C++ application can directly inside a web browser provided that WASM is supported (it is supported on most popular browsers).

It provides an implementation for most popular libraries, including SDL2, SDL2_mixer, OpenGL (WebGL 2.0/3.0), and GLEW, amongst others. Of course, we are also using SDL-GPU to simplify things for us, so that makes things slightly tricky.

## Preparing the toolchain

This is fairly standard. Clone the repo, and prepare the toolchain as given in the official documentation on a Linux system (WSL/Debian/Arch whatever...)

```bash

git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh

```

Now, you should have both the `em++` (The C++ compiler) and `emcc` (The C compiler) working.

## Compiling SDL2-GPU

Alright, so the first thing we have to do is compile SDL2-GPU. Thankfully, SDL2-GPU is fairly standard and does not use any hacks, so essentially we do the following:

```bash

git clone https://github.com/grimfang4/sdl-gpu.git
cd sdl-gpu

emcc -r -O2 -s USE_WEBGL2=1 -s USE_SDL=2 -DSDL_GPU_DISABLE_GLES_1 -DSDL_GPU_DISABLE_GLES_2 -DSDL_GPU_DISABLE_OPENGL -DSDL_GPU_USE_BUFFER_RESET -Iinclude -Isrc/externals/stb_image -Isrc/externals/stb_image_write src/externals/stb_image_write/stb_image_write.c src/SDL_gpu_shapes.c src/SDL_gpu_matrix.c src/renderer_GLES_3.c src/SDL_gpu_renderer.c src/SDL_gpu.c -o ./libSDL2_gpu.bc -Wno-incompatible-function-pointer-types

```

Right, that last command is a bit big, but we compile with WebGL, SDL, and tell SDL2-GPU to compile only with GLES3 support. We think compile the appropriate *.c files, and suppress the "-Wno-incompatible-function-pointer-types" warning (as it causes compilation failure).

Now, we have a file called `libSDL2_gpu.bc` which is LLVM bytecode. It's a static library, like a `.a` file.

## Modifying your program to work with Emscripten

Again, straight forward, it just required some modifications to `main.cpp`:

```c++

#ifdef EMSCRIPTEN_IMPLEMENTATION
#include <emscripten.h>
#endif

...

int main(void)
{
    ...
#ifdef EMSCRIPTEN_IMPLEMENTATION
    emscripten_set_main_loop(MainLoop, 0, 0);
#else
    MainLoop();
#endif
    return 0;
}

...

void MainLoop(void)
{
    ...
    while(running)
    {
        ...
        GPU_ClearColor(...)
        game.Frame(delta_time);
        ...

        #ifdef EMSCRIPTEN_IMPLEMENTATION
        /* Important: Not doing this will crash the browser. The sleep allows the browser to do the rendering and event work. */
        emscripten_sleep(16);
        #endif
        ...
    }
}

```

Now, we are ready to compile the program for Emscripten.

## Compiling the program for Emscripten

This is a self-explanatory script, important bits are allowing memory growth, and asyncify. We also enable the macro "-DEMSCRIPTEN_IMPLEMENTATION" which is what we are checking for in our code to have Emscripten specific code.

```bash

export EMSCRIPTEN_SDL2_INCLUDE_DIR="/home/sgautam/emsdk/upstream/emscripten/cache/sysroot/include/SDL2"
export EMSCRIPTEN_SDL_GPU_STATIC_LIB="/home/sgautam/sdl-gpu/libSDL2_gpu.bc"
export GLM_INCLUDE_DIR="./lib/glm/include"
export CFLAGS="-Wno-c++11-narrowing"
export CPP_FILES="main.cpp Asteroid.cpp BitmapFont.cpp Game.cpp GameHud.cpp Particle.cpp Projectile.cpp Shader.cpp SpaceShip.cpp VGA437.cpp"
export EMFLAGS="-s USE_SDL=2 -s USE_SDL_MIXER=2 -s USE_OGG=1 -s USE_VORBIS=1 -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH=1 -sASYNCIFY"
em++ -I$EMSCRIPTEN_SDL2_INCLUDE_DIR -I$GLM_INCLUDE_DIR $CFLAGS $CPP_FILES $EMSCRIPTEN_SDL_GPU_STATIC_LIB -DEMSCRIPTEN_IMPLEMENTATION $EMFLAGS -o index.html --preload-file ./resources

```

And we now have an index.html with some WASM and JS files that could be hosted on an HTTP server. I hit up `python -m http.server` and had my program hosted on the localhost.

## WebGL woes

So, the first time I ran Asteroids++ under WebGL, WebGL crashed with various shader errors, the prominent issues were:

- Version errors (most of my shader code started with `"#version 330"`)
- Using terms like `attribute` and `varying` versus `in` and `out`.
- Floating point precision errors.

WebGLSL is fairly compatible with GLSL, so most of the code was not an issue.

To work with the version issue, I added the following code to Shader.cpp to *dynamically add the version* to the code and removed the shader version from all shader code:

```c++

	std::string source_shader_with_version = std::string(source);
#ifdef EMSCRIPTEN_IMPLEMENTATION
	source_shader_with_version = "#version 300 es\nprecision mediump float;\n" + source_shader_with_version;
#else
	source_shader_with_version = "#version 330\n" + source_shader_with_version;
#endif

```

Now that, combined with some explicit floating point numbers (WebGL screams when it sees "1/512.0" instead of "1.0/512.0"), and we had the program working *mostly*.

## Music

The initial format used by the music files was MP3, but Emscripten's SDL2_mixer implementation does not support that. So essentially, we were left with turning all MP3s into OGGs.  

## That's it.

And yeah, that was it. Final step was writing a custom index.html with Bootstrap5 to make it look a little more presentable.

export EMSCRIPTEN_SDL2_INCLUDE_DIR="/home/sgautam/emsdk/upstream/emscripten/cache/sysroot/include/SDL2"
export EMSCRIPTEN_SDL_GPU_STATIC_LIB="/home/sgautam/sdl-gpu/libSDL2_gpu.bc"
export GLM_INCLUDE_DIR="./lib/glm/include"
export CFLAGS="-Wno-c++11-narrowing"
export CPP_FILES="main.cpp Asteroid.cpp BitmapFont.cpp Game.cpp GameHud.cpp Particle.cpp Projectile.cpp Shader.cpp SpaceShip.cpp VGA437.cpp"
em++ -I$EMSCRIPTEN_SDL2_INCLUDE_DIR -I$GLM_INCLUDE_DIR $CFLAGS $CPP_FILES $EMSCRIPTEN_SDL_GPU_STATIC_LIB -DEMSCRIPTEN_IMPLEMENTATION -s USE_SDL=2 -s USE_SDL_MIXER=2 -s FULL_ES3=1 -o index.html --preload-file ./resources
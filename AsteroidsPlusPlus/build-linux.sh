export CXX="g++"
export CFLAGS="-Wall -I /usr/include/SDL2/ -c"
export CLD="g++"
export CLDFLAGS="-lm -lstdc++ -lSDL2 -lSDL2_gpu -lGL -lSDL2_mixer"

$CXX $CFLAGS Asteroid.cpp -o Asteroid.o
$CXX $CFLAGS BitmapFont.cpp -o BitmapFont.o
$CXX $CFLAGS Game.cpp -o Game.o
$CXX $CFLAGS GameHud.cpp -o GameHud.o
$CXX $CFLAGS Projectile.cpp -o Projectile.o
$CXX $CFLAGS Shader.cpp -o Shader.o
$CXX $CFLAGS SpaceShip.cpp -o SpaceShip.o
$CXX $CFLAGS Particle.cpp -o Particle.o
$CXX $CFLAGS VGA437.cpp -o VGA437.o
$CXX $CFLAGS main.cpp -o main.o

$CLD -o ./AsteroidsPlusPlus.elf main.o Asteroid.o BitmapFont.o Game.o GameHud.o Projectile.o Shader.o SpaceShip.o VGA437.o Particle.o $CLDFLAGS

rm *.o

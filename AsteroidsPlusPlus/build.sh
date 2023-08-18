export CXX="g++"
export CFLAGS="-Wall -c -I /usr/include/SDL2/"
export LD="g++"
export LDFGLAGS="-lc -lm -lstdc++ -lSDL2 -lGL -L/usr/lib/libSDL2_gpu.a -lSDL_mixer -fstack-protector"
set -xe

$CXX $CFLAGS Asteroid.cpp -o Asteroid.o
$CXX $CFLAGS BitmapFont.cpp -o BitmapFont.o
$CXX $CFLAGS Game.cpp -o Game.o
$CXX $CFLAGS GameHud.cpp -o GameHud.o
$CXX $CFLAGS Projectile.cpp -o Projectile.o
$CXX $CFLAGS Shader.cpp -o Shader.o
$CXX $CFLAGS SpaceShip.cpp -o SpaceShip.o
$CXX $CFLAGS -x c++ VGA437.inc -o VGA437.o
$CXX $CFLAGS main.cpp -o main.o

$LD -o ./AsteroidsPlusPlus main.o Asteroid.o BitmapFont.o Game.o GameHud.o Projectile.o Shader.o SpaceShip.o VGA437.o $LDFLAGS

rm *.o

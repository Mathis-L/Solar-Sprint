all:
	g++ -I src/include -L src/lib -o SolarSprint Main.cpp Game.cpp Texte.cpp Player.cpp Obstacle.cpp Coin.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
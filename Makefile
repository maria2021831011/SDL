all:
	
	g++ -I src/include -L src/lib add.cpp -o a -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf  & .\a
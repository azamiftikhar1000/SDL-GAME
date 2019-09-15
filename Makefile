CONFIG = `sdl2-config --cflags --libs` -lSDL2_ttf

snake: game.cpp main.cpp
	@# Compile game
	
	@echo "Compiling the game..."
	@g++ -o snake game.cpp main.cpp $(CONFIG)
	@echo "Compilation Done."
	

	@# Instruction to run the game
	
	@echo "\n--------------------------------"
	@echo "To run the Snake game, type:"
	@echo "./snake"
	@echo "--------------------------------"


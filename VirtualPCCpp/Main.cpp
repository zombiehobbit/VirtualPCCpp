#include <SDL.h>
#include <vector>
#include <iostream>

#include "NSSDL.h"
#include "SDLWindow.h"
#include "Memory.h"
#include "CPU.h"
#include "PC.h"
#include "GPU.h"

int width = 320;
int height = 240;

bool quit = false;

typedef unsigned char byte;

int main(int argc, char* argv[]) {
	SDL_Event event;

	SDL_Window* window2;
	SDL_Surface* surface2;
	SDL_Surface* pixelSpace2;
	SDL_Renderer* renderer2;
	SDL_Texture* texture2;
	unsigned int* pixels2;

	SDLWindow pc1W(window2, surface2, pixelSpace2, renderer2, texture2, pixels2);

	Memory ram1(256);
	Memory hdd1(2048);

	NSSDL::initSDL(pc1W, width, height);

	GPU gpu1(255, 1024, pc1W);

	CPU cpu1(1024, ram1, gpu1);

	//Characters:
	//0:  0, 1:  1, 2:  2, 3:  3, 4:  4, 5:  5, 6:  6, 7:  7, 8:  8, 9:  9,
	//!: 10, #: 11, %: 12, (: 13, ): 14, `: 15, -: 16, [: 17, ]: 18, ^: 19,
	//_: 20, {: 21, }: 22, +: 23, =: 24, >: 25, <: 26, \: 27, |: 28, /: 29,
	//:: 30, ;: 31, *: 32, &: 33, ?: 34, A: 35, B: 36, C: 37: D: 38, E: 39,
	//F: 40, G: 41, H: 42, I: 43, J: 44, K: 45, L: 46, M: 47, N: 48, O: 49,
	//P: 50, Q: 51, R: 52, S: 53, T: 54, U: 55, V: 56, W: 57, X: 58, Y: 59,
	//Z: 60, x: 61

	//Prints B at charcater position 0, 0
	cpu1.cache.memory[100] = 0;
	cpu1.cache.memory[101] = 1;
	cpu1.cache.memory[102] = 255;

	cpu1.cache.memory[0] = 1;
	cpu1.cache.memory[1] = 100;
	cpu1.cache.memory[3] = 2;
	cpu1.cache.memory[4] = 101;
	cpu1.cache.memory[6] = 20;
	cpu1.cache.memory[7] = 40;

	cpu1.cache.memory[8] = 8;
	cpu1.cache.memory[9] = 100;
	cpu1.cache.memory[11] = 7;
	cpu1.cache.memory[12] = 101;

	cpu1.cache.memory[14] = 2;
	cpu1.cache.memory[15] = 102;
	cpu1.cache.memory[17] = 26;
	cpu1.cache.memory[18] = 31;
	cpu1.cache.memory[19] = 1;
	cpu1.cache.memory[20] = 0;

	PC pc1(cpu1, ram1, hdd1, gpu1.screen);

	while (quit == false) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			}
		}	
		pc1.cpu.tick();
		gpu1.execute();
		gpu1.tick();
		NSSDL::updateSDL(pc1.screen);
	}

	NSSDL::cleanSDL(pc1.screen);

	SDL_Quit();

	return 0;
}
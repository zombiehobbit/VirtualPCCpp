#include "GPU.h"


GPU::GPU(int commandBufferSize, int vRamSize, int coreCount, int commandArgBufferSize, SDLWindow* screen_, Memory& ram_)
	:
	commandBuffer(Memory(commandBufferSize)),
	commandArgBuffer(Memory(commandArgBufferSize)),
	vRam(Memory(vRamSize)),
	ram(ram_),
	screen(screen_)
{
	for (int x = 0; x < coreCount; x++) {
		cores.emplace_back(vRam, screen_, 256, x, 0);
	}
	
	for(int y = 0; y < 30; y++) {
		for(int x = 0; x < 53; x++) {
			screenCharacter tmp;
			tmp.x = x;
			tmp.y = y;
			tmp.r = 7;
			tmp.g = 7;
			tmp.b = 3;
			tmp.rB = 0;
			tmp.gB = 0;
			tmp.bB = 0;
			tmp.characterID = 95;
			screenCharacters.push_back(tmp);
		}
	}
}

void GPU::loadFont() {
	SDL_Surface *bmpSurf = SDL_LoadBMP("Fonts/Basic_Font.bmp");
	font = SDL_CreateTextureFromSurface(screen->renderer, bmpSurf);
	SDL_FreeSurface(bmpSurf);
}

void GPU::setCharID(byte x, byte y, byte id) {
	screenCharacters[y * 53 + x].characterID = id; 
}

void GPU::setCharCB(byte x, byte y, byte cB) {
	screenCharacters[y * 53 + x].rB = (cB >> 5) & 0b00000111;
	screenCharacters[y * 53 + x].gB = (cB >> 2) & 0b00111;
	screenCharacters[y * 53 + x].bB = cB & 0b11;
}

void GPU::setCharCF(byte x, byte y, byte cF) {
	screenCharacters[y * 53 + x].r = (cF >> 5) & 0b00000111;
	screenCharacters[y * 53 + x].g = (cF >> 2) & 0b00111;
	screenCharacters[y * 53 + x].b = cF & 0b11;
}

void GPU::updateScreen() {
	SDL_Rect source;
	SDL_Rect target;
	source.w = 6;
	source.h = 8;
	target.w = 6;
	target.h = 8;
	for(int y = 0; y < 30; y++) {
		for(int x = 0; x < 53; x++) {
			source.x = 7 * 6;
			source.y = 6 * 8;

			target.x = x * 6;
			target.y = y * 8;

			SDL_SetTextureColorMod(font, screenCharacters[y * 53 + x].rB * 36.428, screenCharacters[y * 53 + x].gB * 36.428, screenCharacters[y * 53 + x].bB * 85);

			SDL_RenderCopy(screen->renderer, font, &source, &target);

			source.x = (screenCharacters[y * 53 + x].characterID % 10) * 6;
			source.y = (screenCharacters[y * 53 + x].characterID / 10) * 8;
			

			SDL_SetTextureColorMod(font, screenCharacters[y * 53 + x].r * 36.428, screenCharacters[y * 53 + x].g * 36.428, screenCharacters[y * 53 + x].b * 85);

			SDL_RenderCopy(screen->renderer, font, &source, &target);
		}
	}
	SDL_RenderPresent(screen->renderer);
}

void convertByte(int number, byte& b1, byte& b2, byte& b3) {
	b1 = number & 0xff;
	b2 = (byte)(number >> 8);
	b3 = (byte)(number >> 16);
}

void GPU::startCores() {
	for (int i = 0; i < cores.size(); i++) {
		if (tasks.size() > 0) {
			cores[i].programCounter = programCounter;
			cores[i].idX = tasks[0].x;
			cores[i].idY = tasks[0].y;
			tasks.erase(tasks.begin());
			cores[i].halt = false;
		}else{
			cores[i].idX = 0;
			cores[i].idY = 0;
		}
	}
}

void GPU::tick() {
	for (int i = 0; i < cores.size(); i++) {
		if (cores[i].halt == true) {
			if (tasks.size() > 0) {
				cores[i].programCounter = programCounter;
				cores[i].idX = tasks[0].x;
				cores[i].idY = tasks[0].y;
				tasks.erase(tasks.begin());
				cores[i].halt = false;
			}
		}else{
			cores[i].tick();
		}		
	}
}
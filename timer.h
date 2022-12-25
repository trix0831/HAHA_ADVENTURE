#include<iostream>
#include<cstdlib>
#include "SDL.h"
#include "SDL2/SDL_ttf.h"
#include <string>
#include <sstream>
using namespace std;
SDL_Color TextColor={255,255,255};

class Timer{
	public:
		Timer();
		void Init(SDL_Surface* sf,SDL_Renderer* rd,string str);
	private:
		SDL_Texture* texture;
		TTF_Font* font;
		int Width;
		int Height;
		int posx;
		int posy;
		SDL_Rect pos;
		
};
Timer::Timer():posx(500),posy(20){
	TTF_Init();
}
void Timer::Init(SDL_Surface* sf,SDL_Renderer* rd,string str){
	//SDL_RenderClear(rd);
	font=TTF_OpenFont("./img/lazy.ttf",28);
	sf=TTF_RenderText_Solid(font,str.c_str(),TextColor);
	Width=sf->w;
	Height=sf->h;
	pos={posx,posy,Width,Height};
	texture=SDL_CreateTextureFromSurface(rd,sf);
	SDL_RenderSetViewport(rd,&pos); 
	SDL_RenderCopy(rd,texture,NULL,NULL);
	SDL_RenderPresent(rd); 
}

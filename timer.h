#include<iostream>
#include<cstdlib>
#include "SDL.h"
#include "SDL2/SDL_ttf.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;
SDL_Color TextColor={255,255,255};

class Timer{
	public:
		Timer();
		void Init(SDL_Renderer* rd,string str);
		void update(SDL_Renderer* rd);
		void Start();
		void Resume();
		void Pause(); 
		void Stop();
		Uint32 GetTime();
	private:
		SDL_Surface* surface;
		SDL_Texture* texture;
		TTF_Font* font;
		int Width;
		int Height;
		int posx;
		int posy;
		SDL_Rect pos;
		bool started;
		bool paused;
		Uint32 TimeStarted;
		Uint32 TimePaused;
		stringstream timetxt;
		
};
Timer::Timer():posx(500),posy(20),TimeStarted(0),TimePaused(0){
	TTF_Init();
	started=false;
	paused=false;
}
void Timer::Init(SDL_Renderer* rd,string str){
	font=TTF_OpenFont("./img/lazy.ttf",28);
	surface=TTF_RenderText_Solid(font,str.c_str(),TextColor);
	Width=surface->w;
	Height=surface->h;
	pos={posx,posy,Width,Height};
	texture=SDL_CreateTextureFromSurface(rd,surface);
	SDL_RenderSetViewport(rd,&pos);
	SDL_RenderCopy(rd,texture,NULL,NULL);
	SDL_RenderPresent(rd); 
}
void Timer::update(SDL_Renderer* rd){
	timetxt.str("");
	timetxt<<setw(2)<<setfill('0')<<GetTime()/(1000*60)<<':';
	timetxt<<setw(2)<<setfill('0')<<(GetTime()/(1000))%60;
	Init(rd,timetxt.str());
}
void Timer::Start(){
	started=true;
	TimeStarted=SDL_GetTicks();
}
void Timer::Pause(){
	if(started&&(!paused)){
		paused=true;
	}
	TimePaused=SDL_GetTicks()-TimeStarted;
}
void Timer::Resume(){
	if(started&&paused){
		paused=false;	
	}
	TimeStarted=SDL_GetTicks()-TimePaused;
}
void Timer::Stop(){
	started=false;
	TimeStarted=0;
	TimePaused=0;
}
Uint32 Timer::GetTime(){
	if(started){
		if(paused){
			return TimePaused;
		}
		else{
			return SDL_GetTicks()-TimeStarted;
		}
	}
	else{
		return 0;
	}
}


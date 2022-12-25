#include<iostream>
#include<cstdlib>
#include "SDL.h"
#include "SDL2/SDL_ttf.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;
SDL_Color TextColor={254,254,254};//Color of time

class Timer{
	public:
		Timer();
		void Init(SDL_Renderer* rd,string str);
		void Init2(SDL_Renderer* rd,string str);
		void update(SDL_Renderer* rd);
		void Start();
		void Resume();
		void Pause(); 
		void Stop();
		Uint32 GetTime();
	private:
		SDL_Surface* surface;
		SDL_Surface* surface2;
		
		SDL_Texture* texture;
		SDL_Texture* texture2;
		TTF_Font* font;
		int Width;
		int Height;
		int posx;
		int posy;
		
		SDL_Rect pos;
		SDL_Rect pos2;
		
		bool started;
		bool paused;
		Uint32 TimeStarted;
		Uint32 TimePaused;
		Uint32 TimeMinimum;
		
		stringstream timetxt;
		stringstream timemin;
		
};
Timer::Timer():posx(600),posy(20),TimeStarted(0),TimePaused(0),TimeMinimum(0){
	TTF_Init();
	started=false;
	paused=false;
	font=TTF_OpenFont("./img/lazy.ttf",28);
}
void Timer::Init(SDL_Renderer* rd,string str){
	surface=TTF_RenderText_Solid(font,str.c_str(),TextColor);
	Width=surface->w;
	Height=surface->h;
	pos={posx,posy,Width,Height};
	texture=SDL_CreateTextureFromSurface(rd,surface);
	SDL_RenderSetViewport(rd,&pos);
	SDL_RenderCopy(rd,texture,NULL,NULL);
	//SDL_RenderPresent(rd); 
}
void Timer::Init2(SDL_Renderer* rd,string str){
	surface2=TTF_RenderText_Solid(font,str.c_str(),TextColor);
	Width=surface->w;
	Height=surface->h;
	pos2={posx,posy+40,Width,Height};
	texture2=SDL_CreateTextureFromSurface(rd,surface2);
	SDL_RenderSetViewport(rd,&pos2);
	SDL_RenderCopy(rd,texture2,NULL,NULL);
	//SDL_RenderPresent(rd); 
}
void Timer::update(SDL_Renderer* rd){
	timetxt.str("");
	timetxt<<setw(2)<<setfill('0')<<GetTime()/(1000*60)<<':';
	timetxt<<setw(2)<<setfill('0')<<(GetTime()/(1000))%60<<'.';
	timetxt<<setw(3)<<setfill('0')<<GetTime()%1000;
	timemin.str("");
	timemin<<setw(2)<<setfill('0')<<TimeMinimum/(1000*60)<<':';
	timemin<<setw(2)<<setfill('0')<<(TimeMinimum/(1000))%60<<'.';
	timemin<<setw(3)<<setfill('0')<<TimeMinimum%1000;
	Init(rd,timetxt.str());
	Init2(rd,timemin.str());
}
void Timer::Start(){
	if(!started){
		started=true;	
	}
	else{
		return;
	}
	TimeStarted=SDL_GetTicks();
}
void Timer::Pause(){
	if(started&&(!paused)){
		paused=true;
	}
	else{
		return;
	}
	TimePaused=SDL_GetTicks()-TimeStarted;
}
void Timer::Resume(){
	if(started&&paused){
		paused=false;	
	}
	else{
		return;
	}
	TimeStarted=SDL_GetTicks()-TimePaused;
}
void Timer::Stop(){
	
	if(TimeMinimum==0){
		TimeMinimum=GetTime();
	}
	else if(GetTime()<TimeMinimum){
		TimeMinimum=GetTime();
	}
	started=false;
	paused=false;
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


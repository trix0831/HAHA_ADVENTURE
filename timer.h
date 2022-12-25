#include<iostream>
#include<cstdlib>
#include "SDL.h"
#include "SDL2/SDL_ttf.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;
SDL_Color TextColor={254,254,254};//Color of time text

class Timer{
	public:
		Timer();//Constructor
		~Timer(){}//Destructor
		
		void Init(SDL_Renderer* rd,string str);//Render elapsed time
		void Init2(SDL_Renderer* rd,string str);//Render minimum elapsed time
		
		void update(SDL_Renderer* rd);//Update time text
		
		void Start();//Start timing
		void Resume();//Resume timing
		void Pause(); //Pause timing
		void Stop();//Stop timing
		Uint32 GetTime();//Get the time(milisec)
	private:
		SDL_Surface* surface;//surface for elapsed time
		SDL_Surface* surface2;//surface for minimum elapsed time
		
		SDL_Texture* texture;//texture for elapsed time
		SDL_Texture* texture2;//texture for minimum elapsed time
		
		TTF_Font* font;
		
		int Width;
		int Height;
		int posx;
		int posy;
		
		SDL_Rect pos;//positon for elapsed time
		SDL_Rect pos2;//positon for minimum elapsed time
		
		bool started;//whether the timer has started
		bool paused;//whether the timer has paused(only change between when started=1, otherwise it remains 0)
		
		Uint32 TimeStarted;//the time elapsed at the instant 
		Uint32 TimePaused;//the time elapsed before pausing
		Uint32 TimeMinimum;//the minimum time elasped
		
		stringstream timetxt;//ss for the elapsed time
		stringstream timemin;//ss for the minimum elapsed time
		
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
}
void Timer::Init2(SDL_Renderer* rd,string str){
	surface2=TTF_RenderText_Solid(font,str.c_str(),TextColor);
	Width=surface->w;
	Height=surface->h;
	pos2={posx,posy+40,Width,Height};
	texture2=SDL_CreateTextureFromSurface(rd,surface2);
	SDL_RenderSetViewport(rd,&pos2);
	SDL_RenderCopy(rd,texture2,NULL,NULL);
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

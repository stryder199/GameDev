/*
 * Window.h
 *
 *  Created on: May 31, 2011
 *      Author: stryder
 */


//=================================
// include guard
#ifndef WINDOW_H_
#define WINDOW_H_

//=================================
// forward declared dependencies
class GameData;
class Camera;

//=================================
// included dependencies
#ifdef __GNUC__ 	//If compiler cygwin/gcc on Windows XP and for gcc 32-bit and 64-bit Linux
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#else 				//If Borland and Microsoft compilers
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#endif

//Our window
class Window
{
private:

	SDL_Surface* screen;

	//Whether the window is windowed or not
	bool windowed;

	//Whether the window is fine
	bool windowOK;

	int SCREEN_HEIGHT, SCREEN_WIDTH, LEVEL_HEIGHT, LEVEL_WIDTH;

	Camera* myCamera;

public:

	static const int SCREEN_BPP = 32;
	static const int SCREEN_VEL = 1000;
	static const int FPS = 60;

	//Constructor
	Window();

	//Deconstructor
	~Window();

	//Handle window events
	void handle_events( GameData *game_data );

	//Turn fullscreen on/off
	void toggle_fullscreen();

	//Check if anything's wrong with the window
	bool error();

	int get_ScreenWidth();

	int get_ScreenHeight();

	int get_LevelWidth();

	int get_LevelHeight();

	int get_ScreenVel();

	SDL_Surface* get_screen();

	Camera* get_camera();
};

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip );
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, Camera* camera );
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
void apply_text_surface( int  x, int y, SDL_Surface* destination, const char* text, TTF_Font* font, SDL_Color textColour );

#endif /* WINDOW_H_ */

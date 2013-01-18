/*
 * myButton.h
 *
 *  Created on: Oct 19, 2011
 *      Author: stryder
 */

//=================================
// include guard
#ifndef BUTTON_H_
#define BUTTON_H_

//=================================
// forward declared dependencies
class Window;
class GameData;

//=================================
// included dependencies
#ifdef __GNUC__ 	//If compiler cygwin/gcc on Windows XP and for gcc 32-bit and 64-bit Linux
#include "SDL/SDL.h"
#else 				//If Borland and Microsoft compilers
#include "SDL.h"
#endif

class myButton
{
private:

	SDL_Rect* button;

public:

	myButton( SDL_Rect* buttonDimentions );
	bool handle_events( GameData* game_data );
	bool hover_over();
	void show( Window* myWindow );
	void show( Uint8 r, Uint8 g, Uint8 b, Window* myWindow );
	void show( SDL_Surface* image, Window* myWindow );
};

#endif /* BUTTON_H_ */

/*
 * MiniMap.h
 *
 *  Created on: Jan 1, 2012
 *      Author: stryder
 */

//=================================
// include guard
#ifndef MINIMAP_H_
#define MINIMAP_H_

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

#include <list>

class MiniMap{

private:

	bool toggle;
	SDL_Rect* minimapRect;

public:

	MiniMap( GameData* game_data );

	void show( GameData* game_data, Window* myWindow );

	void handle_events( GameData* game_data, Window* myWindow );

};

int xRelMM( int x, SDL_Rect* miniMap, Window* myWindow );

int yRelMM( int y, SDL_Rect* miniMap, Window* myWindow );

int MMRelx( int x, SDL_Rect* miniMap, Window* myWindow );

int MMRely( int y, SDL_Rect* miniMap, Window* myWindow );

#endif /* MINIMAP_H_ */

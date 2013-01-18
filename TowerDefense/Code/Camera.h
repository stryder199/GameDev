/*
 * Camera.h
 *
 *  Created on: Jun 3, 2011
 *      Author: stryder
 */

//=================================
// include guard
#ifndef CAMERA_H_
#define CAMERA_H_

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

class Camera
{
private:

	//The dimentions of the camera
	int x, y, w, h;

	//The Velocity of the camera
	float xVel, yVel;

	bool leftScroll, rightScroll, topScroll, botScroll;

public:
	Camera( Window *myWindow );

	void handle_input( Window *myWindow, GameData *game_data );

	void move(Uint32 deltaTicks, Window* myWindow );

	void moveTo( Window* myWindow, int xCord, int yCord );

	int getX();
	int getY();
	int getW();
	int getH();
	void setX( int var );
	void setY( int var );
	void resetCamera();
};

#endif /* CAMERA_H_ */

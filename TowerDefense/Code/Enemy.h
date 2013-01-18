/*
 * Unit.h
 *
 *  Created on: May 31, 2011
 *      Author: stryder
 */

//=================================
// include guard
#ifndef UNIT_H_
#define ENEMY_H_

//=================================
// forward declared dependencies
class Window;
class GameData;
class Timer;
class Resources;
class UI;

//=================================
// included dependencies
#ifdef __GNUC__ 	//If compiler cygwin/gcc on Windows XP and for gcc 32-bit and 64-bit Linux
#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#else 				//If Borland and Microsoft compilers
#include "SDL.h"
#include "SDL_gfxPrimitives.h"
#endif

#include <sstream>
#include <vector>

//The Enemy
class Enemy
{
private:
	//The timer that keeps an enemy slowed for 5 secs
	Timer *slowedTimer;

	//The X and Y offsets of the Enemy
	int x, y;

	//The Velocity of the Enemy
	float xVel, yVel, prev_xVel, prev_yVel;

	bool selected, slowed;

	int maxhealth, health, attack, defense, ID, frame;

	SDL_Surface **frame1, **frame2, **frame3, **frame4, **frame5, **frame6;

public:

	//Contants
	static const int ENEMY_WIDTH = 40;
	static const int ENEMY_HEIGHT = 40;

	//Initializes the variables
	Enemy( int id_number, int enemy_health, int velocity, int enemy_defense, Window* myWindow, GameData* game_data );

	//Moves the Enemy
	void move(Uint32 deltaTicks, Window* myWindow);
	void menuMove( Uint32 deltaTicks, Window* myWindow );

	//Shows the Enemy on the screen
	void show( Window* myWindow, GameData* game_data );

	//Set member Vars
	void set_camera( Window* myWindow );
	void setX( int val );
	void setY( int val );
	void offSetY( int val );
	void offsetHealth( int x );
	void setSlowed();

	//Get member Vars
	int getX();
	int getY();
	int getEnemyNum();
	bool isSelected();
	int getHealth();
	int getMaxHealth();
	float enemyAngle();
	bool getSlowed();
};

SDL_Surface* getEnemyRotatedImage( int itsOrientation, SDL_Surface* image[]);
void applyEnemies( GameData *game_data , Resources* myResources, Timer *delta, int *enemy_counter, UI* myUI, Window *myWindow );
Enemy* spawnEnemy( GameData *game_data, int &enemy_counter, int &ID_number, int health, int velocity, int defense, Window *myWindow );

#endif /* Enemy_H_ */

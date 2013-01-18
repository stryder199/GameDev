/*
 * Tower.h
 *
 *  Created on: Oct 6, 2011
 *      Author: stryder
 */


//=================================
// include guard
#ifndef TOWER_H_
#define TOWER_H_

//=================================
// forward declared dependencies
class Window;
class GameData;
class Enemy;
class UI;
class Camera;

//=================================
// included dependencies
#ifdef __GNUC__ 	//If compiler cygwin/gcc on Windows XP and for gcc 32-bit and 64-bit Linux
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#else 				//If Borland and Microsoft compilers
#include "SDL.h"
#include "SDL_mixer.h"
#endif

#include "Timer.h"
#include <string>
#include <vector>

class Tower
{
private:

	int x, y, range, attackspeed, damage, slow, type, sell_return, buy_cost, upgrade_teir;

	Uint8 laserColorR, laserColorG, laserColorB, laserWidth;

	bool alive, onTopTower;

	SDL_Surface *image, *lastImg, *icon, **rotationArray, *image_path;
	Mix_Chunk *sound;
	bool planted;
	Enemy *target;
	Timer *attackTimer;

	std::string title, description;

public:

	static const int TOWER_WIDTH = 95;
	static const int TOWER_HEIGHT = 95;

	Tower( int type_number, Window *myWindow, GameData *game_data );
	Tower( SDL_Surface *image_path , int type_number, int xvar, int yvar, Mix_Chunk *sound_path, Window *myWindow, GameData *game_data );
	~Tower();
	void show( Window* myWindow, GameData* game_data, UI* myUI );
	void show( SDL_Surface *rotatedImage, Window* myWindow );
	void handleAttack( Window* myWindow );

	bool getPlanted();
	Enemy* getTarget();
	int getRange();
	int getType();
	int getDamage();
	int getAttackSpeed();
	int getSellCost();
	int getBuyCost();
	SDL_Surface* getLastImg();
	int getUpgradeTeir();
	std::string getTitle();
	std::string getDescription();	
	int getX();
	int getY();
	int getX_cam( Window* myWindow );
	int getY_cam( Window* myWindow );
	SDL_Surface* getIcon();

	void setPlanted( bool x );
	void setX( int val );
	void setY( int val );
	void setTarget( Enemy *val );
	void setLastImage( SDL_Surface *img );
	void setUpgradeTeir( int i );
	void setDead();
	void setRange( int var );

	void upgradeTeirPP();
	void handleUpgrades();

	bool isAlive();

	void setTowOntop( bool var );
	bool getTowOntop();	
};

float towerAngle( Tower* tower, Enemy* enemy );
SDL_Surface* getRotatedImage( int itsOrientation, SDL_Surface* image[] );
void applyTowers( Window *myWindow, GameData *game_data, UI* myUI );

#endif /* TOWER_H_ */

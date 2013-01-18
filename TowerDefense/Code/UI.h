/*
 * UI.h
 *
 *  Created on: 2012-01-08
 *      Author: stryder
 */

//=================================
// include guard
#ifndef UI_H_
#define UI_H_

//=================================
// forward declared dependencies
class Tower;
class myButton;
class Enemy;
class GameData;
class Window;
class Resources;
class Tower;
class Enemy;
class MiniMap;
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

#include <sstream>

class UI
{
private:

	Enemy *EnemyFocus;

	Tower *spawnedTower, *TowFocus;

	MiniMap *minimap;

	bool hoverOver;

	int hoverType;

	myButton	*cannonButton, *tankButton, *iceButton, *nukeButton, *laserButton, *sellButton, *sellButton2, *upgradeButton1,
				*upgradeButton2, *upgradeButton3, *upgradeButton4, *upgradeButton5, *upgradePath1, *upgradePath2;

public:

	UI( GameData *game_data );
	void showTopBar( Window *myWindow, GameData *game_data, Resources *myResources );
	void showBotUI( Window *myWindow, GameData *game_data );
	void handle_events( GameData *game_data, Resources *myResources, Window *myWindow );
	void nullFocus();
	Tower *getTowFocus();
	Enemy *getEnemyFocus();

};

Tower *spawnTower( GameData *game_data, int type, Window *myWindow );

int xRelCam( int x, Window *myWindow );
int yRelCam( int y, Window *myWindow );


#endif /* UI_H_ */

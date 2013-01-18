/*
 * GameData.h
 *
 *  Created on: Dec 14, 2011
 *      Author: stryder
 */

//=================================
// include guard
#ifndef GAMEDATA_H_
#define GAMEDATA_H_

//=================================
// forward declared dependencies
class myButton;
class Enemy;
class Tower;
class Window;

//=================================
// included dependencies
#ifdef __GNUC__ 	//If compiler cygwin/gcc on Windows XP and for gcc 32-bit and 64-bit Linux
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#else 				//If Borland and Microsoft compilers
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_rotozoom.h"
#endif

//=================================
//	C++ Headers
#include <cassert>
#include <list>
#include <map>
#include <string>

//=================================
//	Defines
#define SCREEN_SCROLL_WIDTH 5
#define	TOP_MENU_HEIGHT 25
#define	BOT_MENU_HEIGHT 150
#define	MENU_BUTTON_WIDTH 600
#define	UPGRADE_BOX_WIDTH 30
#define	UPGRADE_BOX_HEIGHT 30
#define	PICTURE_SQUARE_HW 60
#define	TOW_BUY_BUTTON_HW 50
#define	MENU_BUTTON_HEIGHT 125
#define	COST_BOX_HEIGHT 30
#define	TOW_BUTTON_BUFFER 10

//Holds all the
class GameData{

private:
	//maps
	std::map<std::string, SDL_Surface*> SurfaceMap;
	std::map<std::string, SDL_Rect*> RectMap;
	std::map<std::string, TTF_Font*> FontMap;
	std::map<std::string, Mix_Chunk*> SoundFXMap;
	std::map<std::string, SDL_Surface**> RotatedArray;
	std::map<std::string, SDL_Color> FontColorMap;

	//Creates the vector to hold the units and towers
	std::list<Enemy*>* allEnemies;
	std::list<Tower*>* allTowers;

	//Game music
	Mix_Music* Music;

	//Event Structure
	SDL_Event* event;

	Tower** sampleTowers;

public:

	GameData();
	~GameData();

	SDL_Event* get_EventStruc();

	std::list<Enemy*>* get_AllEnemies();
	std::list<Tower*>* get_AllTowers();

	SDL_Surface* get_surface( std::string key );
	SDL_Rect* get_rect( std::string key );
	TTF_Font* get_font( std::string key );
	Mix_Chunk* get_sound( std::string key );
	SDL_Color get_color( std::string key );
	Mix_Music* get_music();
	SDL_Surface** get_surfaceArray( std::string key );
	Tower* getSampleTower( int towerType );

	void add_surface( std::string key, SDL_Surface* surface );
	void add_rect( std::string key, SDL_Rect* rect );
	void add_font( std::string key, TTF_Font* font );
	void add_sound( std::string key, Mix_Chunk* sfx );
	void add_color( std::string key, SDL_Color mycolor );
	void add_surfaceArray( std::string key, SDL_Surface** surfaceArray );

	void set_music( Mix_Music* song );

	void initAllRectCoords( Window *myWindow );
	void initAllSurfaceArrays();
	bool load_files( Window *myWindow );
	void initSampleTowers( Window *myWindow );

	void load_rotatedSurfaces( std::string key, SDL_Surface* image );
	void load_rect( std::string key, int x, int y, int w, int h );
	void load_image( std::string filename, std::string key );
	void load_sound( char* filename, std::string key, int default_volume );
	void load_font( char* filename, std::string key, int font_size );

};



#endif /* GAMEDATA_H_ */

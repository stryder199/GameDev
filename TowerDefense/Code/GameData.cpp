/*
 * GameData.cpp
 *
 *  Created on: Dec 14, 2011
 *      Author: stryder
 */

#include "GameData.h"
#include "myButton.h"
#include "Window.h"
#include "Enemy.h"
#include "Tower.h"

GameData::GameData()
{
	allTowers = new std::list<Tower*>();
	allEnemies = new std::list<Enemy*>();
	Music = NULL;
	event = new SDL_Event();

	sampleTowers = new Tower*[5];
}

GameData::~GameData()
{

}

SDL_Event* GameData::get_EventStruc()
{
	return event;
}

std::list<Enemy*>* GameData::get_AllEnemies()
{
	return allEnemies;
}

std::list<Tower*>* GameData::get_AllTowers()
{
	return allTowers;
}

Tower* GameData::getSampleTower( int towerType )
{
	return sampleTowers[towerType - 1];
}

SDL_Surface* GameData::get_surface( std::string key )
{
	assert( SurfaceMap.find(key)->second != NULL );

	return SurfaceMap.find(key)->second;
}

SDL_Rect* GameData::get_rect( std::string key )
{
	assert( RectMap.find(key)->second != NULL );

	return RectMap.find(key)->second;
}

TTF_Font* GameData::get_font( std::string key )
{
	assert( FontMap.find(key)->second != NULL );

	return FontMap.find(key)->second;
}

Mix_Chunk* GameData::get_sound( std::string key )
{
	assert( SoundFXMap.find(key)->second != NULL );

	return SoundFXMap.find(key)->second;
}

SDL_Color GameData::get_color( std::string key )
{
	return FontColorMap.find(key)->second;
}

Mix_Music* GameData::get_music()
{
	return Music;
}

SDL_Surface** GameData::get_surfaceArray( std::string key )
{
	assert( RotatedArray.find(key)->second != NULL );

	return RotatedArray.find(key)->second;
}

void GameData::add_surface( std::string key, SDL_Surface* surface )
{
	assert( surface != NULL );

	SurfaceMap.insert( std::pair<std::string, SDL_Surface*>(key, surface) );
}

void GameData::add_rect( std::string key, SDL_Rect* rect )
{
	assert( rect != NULL );

	RectMap.insert( std::pair<std::string, SDL_Rect*>(key, rect) );
}

void GameData::add_font( std::string key, TTF_Font* font )
{
	assert( font != NULL );

	FontMap.insert( std::pair<std::string, TTF_Font*>(key, font) );
}

void GameData::add_sound( std::string key, Mix_Chunk* sfx )
{
	assert( sfx != NULL );

	SoundFXMap.insert( std::pair<std::string, Mix_Chunk*>(key, sfx) );
}

void GameData::add_color( std::string key, SDL_Color mycolor )
{
	FontColorMap.insert( std::pair<std::string, SDL_Color>(key, mycolor) );
}

void GameData::set_music( Mix_Music* song )
{
	assert( song != NULL );

	Music = song;
}

void GameData::add_surfaceArray( std::string key, SDL_Surface** surfaceArray )
{
	assert( surfaceArray != NULL );

	RotatedArray.insert( std::pair<std::string, SDL_Surface**>( key, surfaceArray ) );
}

void GameData::initAllRectCoords( Window *myWindow )
{
	load_rect( "topMenu", 0, 0, myWindow->get_ScreenWidth(), TOP_MENU_HEIGHT );
	load_rect( "botMenu", myWindow->get_ScreenWidth()/2 - myWindow->get_ScreenWidth()/4, myWindow->get_ScreenHeight() - BOT_MENU_HEIGHT, myWindow->get_ScreenWidth()/2, BOT_MENU_HEIGHT);
	load_rect( "background", 0, BOT_MENU_HEIGHT, myWindow->get_ScreenWidth(), myWindow->get_ScreenHeight() - TOP_MENU_HEIGHT - BOT_MENU_HEIGHT );
	load_rect( "mainMenu", 0, 0, myWindow->get_ScreenWidth(), myWindow->get_ScreenHeight() );
	load_rect( "upgradeBox1Forground", get_rect("botMenu")->x + get_rect("botMenu")->w/3 + 5, get_rect("botMenu")->y + 5, get_rect("botMenu")->w/3 - 10, get_rect("botMenu")->h - 10 );
	load_rect( "playButton", (myWindow->get_ScreenWidth()/2 - MENU_BUTTON_WIDTH/2), myWindow->get_ScreenHeight()/2 - 100, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT );
	load_rect( "instructionButton", (myWindow->get_ScreenWidth()/2 - MENU_BUTTON_WIDTH/2), get_rect("playButton")->y + 150, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT );
	load_rect( "optionButton", (myWindow->get_ScreenWidth()/2 - MENU_BUTTON_WIDTH/2), get_rect("playButton")->y + 300, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT );
	load_rect( "upgradeBox1", get_rect("upgradeBox1Forground")->x + 10, get_rect("upgradeBox1Forground")->y + get_rect("upgradeBox1Forground")->h - 40, UPGRADE_BOX_WIDTH, UPGRADE_BOX_HEIGHT );
	load_rect( "upgradeBox2", get_rect("upgradeBox1Forground")->x + 60, get_rect("upgradeBox1Forground")->y + get_rect("upgradeBox1Forground")->h - 40,	UPGRADE_BOX_WIDTH, UPGRADE_BOX_HEIGHT );
	load_rect( "upgradeBox3", get_rect("upgradeBox1Forground")->x + 110, get_rect("upgradeBox1Forground")->y + get_rect("upgradeBox1Forground")->h - 40, UPGRADE_BOX_WIDTH, UPGRADE_BOX_HEIGHT );
	load_rect( "upgradeBox4", get_rect("upgradeBox1Forground")->x + 160, get_rect("upgradeBox1Forground")->y + get_rect("upgradeBox1Forground")->h - 40, UPGRADE_BOX_WIDTH, UPGRADE_BOX_HEIGHT );
	load_rect( "upgradeBox5", get_rect("upgradeBox1Forground")->x + 210, get_rect("upgradeBox1Forground")->y + get_rect("upgradeBox1Forground")->h - 40, UPGRADE_BOX_WIDTH, UPGRADE_BOX_HEIGHT );
	load_rect( "pictureBorder", get_rect("upgradeBox1Forground")->x + 10, get_rect("upgradeBox1Forground")->y + 10, PICTURE_SQUARE_HW, PICTURE_SQUARE_HW );
	load_rect( "costBox1", get_rect("upgradeBox1Forground")->x + 90, get_rect("upgradeBox1Forground")->y + 30, get_rect("upgradeBox1Forground")->w - 100, COST_BOX_HEIGHT );
	load_rect( "button1", get_rect( "botMenu" )->x + TOW_BUTTON_BUFFER, myWindow->get_ScreenHeight() - 130, TOW_BUY_BUTTON_HW, TOW_BUY_BUTTON_HW );
	load_rect( "button2", get_rect( "botMenu" )->x + TOW_BUTTON_BUFFER, myWindow->get_ScreenHeight() - 60, TOW_BUY_BUTTON_HW, TOW_BUY_BUTTON_HW );
	load_rect( "button3", get_rect( "button1" )->x + TOW_BUTTON_BUFFER + get_rect( "button1" )->w, myWindow->get_ScreenHeight() - 130, TOW_BUY_BUTTON_HW, TOW_BUY_BUTTON_HW );
	load_rect( "button4", get_rect( "button1" )->x + TOW_BUTTON_BUFFER + get_rect( "button1" )->w, myWindow->get_ScreenHeight() - 60, TOW_BUY_BUTTON_HW, TOW_BUY_BUTTON_HW );
	load_rect( "button5", get_rect( "button3" )->x + TOW_BUTTON_BUFFER + get_rect( "button1" )->w, myWindow->get_ScreenHeight() - 130, TOW_BUY_BUTTON_HW, TOW_BUY_BUTTON_HW );
	load_rect( "evolveButton1", get_rect("upgradeBox1Forground")->x + 10, get_rect("upgradeBox1Forground")->y + get_rect("upgradeBox1Forground")->h - 50, get_rect("upgradeBox1Forground")->w/2 - 20, get_rect("upgradeBox1Forground")->h/3 );
	load_rect( "evolveButton2", get_rect("upgradeBox1Forground")->x + get_rect("upgradeBox1Forground")->w/2 + 10, get_rect("upgradeBox1Forground")->y + get_rect("upgradeBox1Forground")->h - 50, get_rect("upgradeBox1Forground")->w/2 - 20, get_rect("upgradeBox1Forground")->h/3 );
	load_rect( "buttonForground", 5 + get_rect("botMenu")->x, get_rect("botMenu")->y + 5, get_rect("botMenu")->w/3 -10, BOT_MENU_HEIGHT - 10 );
	load_rect( "infoForground", get_rect("botMenu")->x + get_rect("botMenu")->w/3*2 + 5, get_rect("botMenu")->y + 5, get_rect("botMenu")->w/3 -10, get_rect("botMenu")->h - 10 );
	load_rect( "miniMapForground", 0, get_rect( "botMenu" )->y,  200, get_rect( "botMenu" )->h );
	load_rect( "miniMap", get_rect( "miniMapForground" )->x + 5, get_rect( "miniMapForground" )->y + 5,  get_rect( "miniMapForground" )->w - 10, get_rect( "miniMapForground" )->h - 10 );
	load_rect( "leftScreenScroll", 0, 0, SCREEN_SCROLL_WIDTH, myWindow->get_ScreenHeight() );
	load_rect( "topScreenScroll", 0 ,0, myWindow->get_ScreenWidth(), SCREEN_SCROLL_WIDTH );
	load_rect( "rightScreenScroll", myWindow->get_ScreenWidth() - SCREEN_SCROLL_WIDTH, 0, SCREEN_SCROLL_WIDTH, myWindow->get_ScreenHeight() );
	load_rect( "botScreenScroll", 0, myWindow->get_ScreenHeight() - SCREEN_SCROLL_WIDTH, myWindow->get_ScreenWidth(), SCREEN_SCROLL_WIDTH );
}

void GameData::initAllSurfaceArrays()
{
	load_rotatedSurfaces( "cannonImages", get_surface( "cannonTower" ) );
	load_rotatedSurfaces( "tankImages", get_surface( "tankTower" ) );
	load_rotatedSurfaces( "iceImages", get_surface( "iceTower" ) );
	load_rotatedSurfaces( "nukeImages", get_surface( "nukeTower" ) );
	load_rotatedSurfaces( "laserImages", get_surface( "laserTower" ) );
	load_rotatedSurfaces( "shipFrame1", get_surface( "shipFrame1" ) );
	load_rotatedSurfaces( "shipFrame2", get_surface( "shipFrame2" ) );
	load_rotatedSurfaces( "shipFrame3", get_surface( "shipFrame3" ) );
	load_rotatedSurfaces( "shipFrame4", get_surface( "shipFrame4" ) );
	load_rotatedSurfaces( "shipFrame5", get_surface( "shipFrame5" ) );
	load_rotatedSurfaces( "shipFrame6", get_surface( "shipFrame6" ) );

}

bool GameData::load_files( Window *myWindow )
{
	Mix_Music *music = NULL;

	//The color of the font
	SDL_Color textColor = { 0xFF, 0xFF, 0xFF };

	//Load all the images into the image map
	//Input pattern -- file Path, key, game_data
	load_image( "Data/Images/BestShipAni.bmp", "shipSprite" );
	load_image( "Data/Images/frame1.bmp", "shipFrame1" );
	load_image( "Data/Images/frame2.bmp", "shipFrame2" );
	load_image( "Data/Images/frame3.bmp", "shipFrame3" );
	load_image( "Data/Images/frame4.bmp", "shipFrame4" );
	load_image( "Data/Images/frame5.bmp", "shipFrame5" );
	load_image( "Data/Images/frame6.bmp", "shipFrame6" );
	load_image( "Data/Images/Monster-40x40.bmp", "enemy" );
	load_image( "Data/Images/enemy2.bmp", "enemy2" );
	load_image( "Data/Images/enemy3.bmp", "enemy3" );
	load_image( "Data/Images/enemy4.bmp", "enemy4" );
	load_image( "Data/Images/CannonTower.bmp", "cannonTower" );
	load_image( "Data/Images/TankTower.bmp", "tankTower" );
	load_image( "Data/Images/NukeTower.bmp", "nukeTower" );
	load_image( "Data/Images/IceTower.bmp", "iceTower" );
	load_image( "Data/Images/LaserTower.bmp", "laserTower" );
	load_image( "Data/Images/CanTow-40x40.bmp", "cannonTowerSmall" );
	load_image( "Data/Images/IceTower-40x40.bmp", "iceTowerSmall" );
	load_image( "Data/Images/TankTow-40x40.bmp", "tankTowerSmall" );
	load_image( "Data/Images/NukeTow-40x40.bmp", "nukeTowerSmall" );
	load_image( "Data/Images/LaserTow-40x40.bmp", "laserTowerSmall" );
	load_image( "Data/Images/spacebackground.jpg", "background" );
	load_image( "Data/Images/GreenMonster.bmp", "enemyBig" );
	load_image( "Data/Images/menubackground.jpg", "menuBackground" );

	if(myWindow->get_ScreenWidth() < 1300)
	{
		int huge_font_size = 72;
		int large_font_size = 50;
		int mid_large_font_size = 36;
		int mid_font_size = 24 ;
		int mid_small_font_size = 20;
		int small_font_size = 15;
		int tiny_font_size = 12;


		//Load all the fonts into the font map
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "huge_robot_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "large_robot_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "mid/large_robot_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "mid_robot_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "mid/small_robot_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "small_robot_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "tiny_robot_ttf", tiny_font_size );

		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "huge_abeakrg_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "large_abeakrg_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "mid/large_abeakrg_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "mid_abeakrg_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "mid/small_abeakrg_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "small_abeakrg_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "tiny_abeakrg_ttf", tiny_font_size );

		load_font( (char*)"Data/Fonts/criticized.ttf", "huge_criticized_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "large_criticized_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "mid/large_criticized_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "mid_criticized_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "mid/small_criticized_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "small_criticized_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "tiny_criticized_ttf", tiny_font_size );

		load_font( (char*)"Data/Fonts/Conviction.ttf", "huge_conviction_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "large_conviction_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "mid/large_conviction_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "mid_conviction_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "mid/small_conviction_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "small_conviction_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "tiny_conviction_ttf", tiny_font_size );
	}
	else if(myWindow->get_ScreenWidth() < 1500)
	{
		int huge_font_size = 72;
		int large_font_size = 50;
		int mid_large_font_size = 36;
		int mid_font_size = 27 ;
		int mid_small_font_size = 20;
		int small_font_size = 18;
		int tiny_font_size = 15;


		//Load all the fonts into the font map
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "huge_robot_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "large_robot_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "mid/large_robot_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "mid_robot_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "mid/small_robot_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "small_robot_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "tiny_robot_ttf", tiny_font_size );

		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "huge_abeakrg_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "large_abeakrg_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "mid/large_abeakrg_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "mid_abeakrg_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "mid/small_abeakrg_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "small_abeakrg_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "tiny_abeakrg_ttf", tiny_font_size );

		load_font( (char*)"Data/Fonts/criticized.ttf", "huge_criticized_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "large_criticized_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "mid/large_criticized_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "mid_criticized_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "mid/small_criticized_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "small_criticized_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "tiny_criticized_ttf", tiny_font_size );

		load_font( (char*)"Data/Fonts/Conviction.ttf", "huge_conviction_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "large_conviction_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "mid/large_conviction_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "mid_conviction_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "mid/small_conviction_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "small_conviction_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "tiny_conviction_ttf", tiny_font_size );
	}
	else if(myWindow->get_ScreenWidth() <= 1700)
	{
		int huge_font_size = 72;
		int large_font_size = 50;
		int mid_large_font_size = 36;
		int mid_font_size = 30 ;
		int mid_small_font_size = 20;
		int small_font_size = 18;
		int tiny_font_size = 12;


		//Load all the fonts into the font map
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "huge_robot_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "large_robot_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "mid/large_robot_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "mid_robot_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "mid/small_robot_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "small_robot_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "tiny_robot_ttf", tiny_font_size );

		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "huge_abeakrg_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "large_abeakrg_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "mid/large_abeakrg_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "mid_abeakrg_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "mid/small_abeakrg_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "small_abeakrg_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "tiny_abeakrg_ttf", tiny_font_size );

		load_font( (char*)"Data/Fonts/criticized.ttf", "huge_criticized_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "large_criticized_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "mid/large_criticized_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "mid_criticized_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "mid/small_criticized_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "small_criticized_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "tiny_criticized_ttf", tiny_font_size );

		load_font( (char*)"Data/Fonts/Conviction.ttf", "huge_conviction_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "large_conviction_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "mid/large_conviction_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "mid_conviction_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "mid/small_conviction_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "small_conviction_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "tiny_conviction_ttf", tiny_font_size );
	}
	else if(myWindow->get_ScreenWidth() < 2000)
	{
		int huge_font_size = 72;
		int large_font_size = 60;
		int mid_large_font_size = 50;
		int mid_font_size = 36;
		int mid_small_font_size = 20;
		int small_font_size = 18;
		int tiny_font_size = 14;


		//Load all the fonts into the font map
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "huge_robot_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "large_robot_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "mid/large_robot_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "mid_robot_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "mid/small_robot_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "small_robot_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "tiny_robot_ttf", tiny_font_size );

		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "huge_abeakrg_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "large_abeakrg_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "mid/large_abeakrg_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "mid_abeakrg_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "mid/small_abeakrg_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "small_abeakrg_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "tiny_abeakrg_ttf", tiny_font_size );

		load_font( (char*)"Data/Fonts/criticized.ttf", "huge_criticized_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "large_criticized_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "mid/large_criticized_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "mid_criticized_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "mid/small_criticized_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "small_criticized_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "tiny_criticized_ttf", tiny_font_size );

		load_font( (char*)"Data/Fonts/Conviction.ttf", "huge_conviction_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "large_conviction_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "mid/large_conviction_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "mid_conviction_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "mid/small_conviction_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "small_conviction_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "tiny_conviction_ttf", tiny_font_size );
	}
	else
	{
		int huge_font_size = 72;
		int large_font_size = 60;
		int mid_large_font_size = 50;
		int mid_font_size = 36;
		int mid_small_font_size = 20;
		int small_font_size = 18;
		int tiny_font_size = 14;


		//Load all the fonts into the font map
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "huge_robot_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "large_robot_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "mid/large_robot_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "mid_robot_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "mid/small_robot_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "small_robot_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/ROBOM___.TTF", "tiny_robot_ttf", tiny_font_size );

		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "huge_abeakrg_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "large_abeakrg_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "mid/large_abeakrg_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "mid_abeakrg_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "mid/small_abeakrg_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "small_abeakrg_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/ABEAKRG.TTF", "tiny_abeakrg_ttf", tiny_font_size );

		load_font( (char*)"Data/Fonts/criticized.ttf", "huge_criticized_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "large_criticized_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "mid/large_criticized_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "mid_criticized_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "mid/small_criticized_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "small_criticized_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/criticized.ttf", "tiny_criticized_ttf", tiny_font_size );

		load_font( (char*)"Data/Fonts/Conviction.ttf", "huge_conviction_ttf", huge_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "large_conviction_ttf", large_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "mid/large_conviction_ttf", mid_large_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "mid_conviction_ttf", mid_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "mid/small_conviction_ttf", mid_small_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "small_conviction_ttf", small_font_size );
		load_font( (char*)"Data/Fonts/Conviction.ttf", "tiny_conviction_ttf", tiny_font_size );
	}

	//Load the sounds into the sound map
	load_sound( (char*)"Data/Sound/Laser1.wav", "laserSound1", 3 );
	load_sound( (char*)"Data/Sound/Laser2.wav", "laserSound2", 3 );
	load_sound( (char*)"Data/Sound/Laser3.wav", "laserSound3", 3 );
	load_sound( (char*)"Data/Sound/strange.wav", "strange", 50 );
	load_sound( (char*)"Data/Sound/upgradefix.wav", "powerup", 50 );

	//Load sound files
	music = Mix_LoadMUS( "Data/Sound/Thorivo.mp3" );

	//Music and sound fx need balancing
	Mix_VolumeMusic(7);

	this->set_music( music );

	this->add_color( "textColor", textColor );

	//If everything loaded fine
	return true;
}

void GameData::initSampleTowers( Window *myWindow )
{
	for(int i=0; i < 5; i++)
	{
		Tower *tower = new Tower( (i + 1), myWindow ,this );
		sampleTowers[i] = tower;
	}
}

void GameData::load_rotatedSurfaces( std::string key, SDL_Surface* image )
{
	SDL_Surface** rotationArray = new SDL_Surface*[180];

	for( int i=0; i < 180; i++)
	{
		SDL_Surface* temp = SDL_ConvertSurface(image, image->format, SDL_SWSURFACE);
		rotationArray[i] = rotozoomSurface( temp, 2*i, 1, 0 );
	}

	add_surfaceArray( key, rotationArray );
}

void GameData::load_rect( std::string key, int x, int y, int w, int h )
{
	SDL_Rect* myRect = new SDL_Rect;
	myRect->x = x;
	myRect->y = y;
	myRect->w = w;
	myRect->h = h;

	add_rect( key, myRect );
}

void GameData::load_image( std::string filename, std::string key )
{
	//The image that's loaded
	SDL_Surface* loadedImage = NULL;

	//The optimized surface that will be used
	SDL_Surface* optimizedImage = NULL;

	//Load the image
	loadedImage = IMG_Load( filename.c_str() );

	//If the image loaded
	if( loadedImage != NULL )
	{
		//Create an optimized surface
		optimizedImage = SDL_DisplayFormat( loadedImage );

		//Free the old surface
		SDL_FreeSurface( loadedImage );

		//If the surface was optimized
		if( optimizedImage != NULL )
		{
			//Color key surface
			SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
		}
	}

	assert( optimizedImage != NULL );

	add_surface( key, optimizedImage );
}

void GameData::load_sound( char* filename, std::string key, int default_volume )
{
	Mix_Chunk* sound = NULL;
	sound = Mix_LoadWAV( filename );
	Mix_VolumeChunk( sound, default_volume );

	assert( sound != NULL );

	add_sound( key, sound );
}

void GameData::load_font( char* filename, std::string key, int font_size )
{
	TTF_Font* font = NULL;
	font = TTF_OpenFont( filename, font_size );

	assert( font != NULL );

	add_font( key, font );
}
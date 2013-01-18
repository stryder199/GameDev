/*
 * MiniMap.cpp
 *
 *  Created on: Jan 1, 2012
 *      Author: stryder
 */

#include "MiniMap.h"
#include "Window.h"
#include "Tower.h"
#include "Enemy.h"
#include "GameData.h"
#include "Camera.h"

MiniMap::MiniMap( GameData* game_data )
{
	toggle = true;
	minimapRect = game_data->get_rect( "miniMap" );
}

void MiniMap::show( GameData* game_data, Window* myWindow )
{
	SDL_FillRect( myWindow->get_screen(), game_data->get_rect( "miniMapForground" ), SDL_MapRGB( myWindow->get_screen()->format, 0x77, 0x77, 0x77 ) );
	SDL_FillRect( myWindow->get_screen(), game_data->get_rect( "miniMap" ), SDL_MapRGB( myWindow->get_screen()->format, 0x00, 0x00, 0x00 ) );

	std::list<Tower*>::iterator it;
	for( it=game_data->get_AllTowers()->begin(); it != game_data->get_AllTowers()->end(); it++)
	{
		filledCircleRGBA( myWindow->get_screen(), xRelMM( (*it)->getX(), game_data->get_rect( "miniMap" ), myWindow ) ,  yRelMM( (*it)->getY(), game_data->get_rect( "miniMap" ), myWindow ), 2, Uint8(0x7F), Uint8(0xFF), Uint8(0x00), Uint8(0xFF));
	}

	std::list<Enemy*>::iterator it2;
	for( it2=game_data->get_AllEnemies()->begin(); it2 != game_data->get_AllEnemies()->end(); it2++)
	{
		filledCircleRGBA( myWindow->get_screen(), xRelMM( (*it2)->getX(), game_data->get_rect( "miniMap" ), myWindow ) ,  yRelMM( (*it2)->getY(), game_data->get_rect( "miniMap" ), myWindow ), 4, Uint8(0xFF), Uint8(0x24), Uint8(0x00), Uint8(0xFF));
	}

	rectangleRGBA( myWindow->get_screen(), xRelMM( myWindow->get_camera()->getX(), minimapRect, myWindow ), yRelMM( myWindow->get_camera()->getY(), minimapRect, myWindow ), xRelMM( myWindow->get_camera()->getX() + myWindow->get_camera()->getW(), minimapRect, myWindow ), yRelMM( myWindow->get_camera()->getY() + myWindow->get_camera()->getH(), minimapRect, myWindow ), 0xFF, 0xFF, 0xFF, 0xFF );
}

void MiniMap::handle_events( GameData* game_data, Window* myWindow )
{
	if( ( game_data->get_EventStruc()->type == SDL_MOUSEBUTTONDOWN ) && ( game_data->get_EventStruc()->button.button == SDL_BUTTON_LEFT ) )
	{
		if( game_data->get_EventStruc()->button.x > minimapRect->x && game_data->get_EventStruc()->button.x < ( minimapRect->x + minimapRect->w) )
		{
			if( game_data->get_EventStruc()->button.y > minimapRect->y && game_data->get_EventStruc()->button.y < ( minimapRect->y + minimapRect->h) )
			{
				myWindow->get_camera()->moveTo( myWindow, MMRelx( game_data->get_EventStruc()->button.x, minimapRect, myWindow ) - myWindow->get_camera()->getW()/2, MMRely( game_data->get_EventStruc()->button.y, minimapRect, myWindow ) - myWindow->get_camera()->getH()/2 );
			}
		}
	}
}

//x coord relative to the mini map
int xRelMM( int x, SDL_Rect* miniMap, Window* myWindow )
{
	float ratio =  0.0f;
	ratio = (float)x/(float)myWindow->get_LevelWidth();
	return(miniMap->x + (int)((float)miniMap->w*ratio));
}

//y coord relative to the mini map
int yRelMM( int y, SDL_Rect* miniMap, Window* myWindow )
{
	float ratio = 0.0f;
	ratio = (float)y/(float)myWindow->get_LevelHeight();
	return(miniMap->y + (int)((float)miniMap->h*ratio));
}

//x minimap click relative to the screen
int MMRelx( int x, SDL_Rect* miniMap, Window* myWindow )
{
	float ratio =  0.0f;
	ratio = (x - miniMap->x) /(float)miniMap->w;
	return((int)((float)myWindow->get_LevelWidth()*ratio));
}

//y minimap click relative to the screen
int MMRely( int y, SDL_Rect* miniMap, Window* myWindow )
{
	float ratio = 0.0f;
	ratio = ( y - miniMap->y )/(float)miniMap->h;
	return((int)((float)myWindow->get_LevelHeight()*ratio));
}

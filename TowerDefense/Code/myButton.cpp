/*
 * myButton.cpp
 *
 *  Created on: Oct 19, 2011
 *      Author: stryder
 */

#include "myButton.h"
#include "GameData.h"
#include "Window.h"
#include "Camera.h"

myButton::myButton( SDL_Rect* buttonDimentions )
{
	button = buttonDimentions;
}

//Was there a button event where the keypress was??
bool myButton::handle_events( GameData* game_data )
{
	if( ( game_data->get_EventStruc()->type == SDL_MOUSEBUTTONDOWN ) && ( game_data->get_EventStruc()->button.button == SDL_BUTTON_LEFT ) )
	{
		if( game_data->get_EventStruc()->button.x > button->x && game_data->get_EventStruc()->button.x < (button->x + button->w))
		{
			if( game_data->get_EventStruc()->button.y > button->y && game_data->get_EventStruc()->button.y < (button->y + button->h))
			{
				return true;
			}
		}
	}
	else
	{
		return false;
	}
	//dummy variable
	return false;
}

bool myButton::hover_over()
{
	int mouse_x, mouse_y;

	SDL_GetMouseState(&mouse_x, &mouse_y);

	if( mouse_x > button->x && mouse_x < (button->x + button->w))
	{
		if( mouse_y > button->y && mouse_y < (button->y + button->h))
		{
			return true;
		}
	}
	else
	{
		return false;
	}
	//dummy return
	return false;
}

void myButton::show( Window* myWindow )
{
	//Apply the button background
	SDL_FillRect( myWindow->get_screen(), button, SDL_MapRGB( myWindow->get_screen()->format, 0x00, 0x00, 0x00 ) );
}

void myButton::show(Uint8 r, Uint8 g, Uint8 b, Window* myWindow)
{
	//Apply the button background
	SDL_FillRect( myWindow->get_screen(), button, SDL_MapRGB( myWindow->get_screen()->format, r, g, b ) );
}

void myButton::show( SDL_Surface* image, Window* myWindow )
{
	//Apply the button background
	SDL_FillRect( myWindow->get_screen(), button, SDL_MapRGB( myWindow->get_screen()->format, 0x00, 0x00, 0x00 ) );
	apply_surface( button->x + button->w/7, button->y + button->h/7, image, myWindow->get_screen() );
}
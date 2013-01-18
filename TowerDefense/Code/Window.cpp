/*
 * Window.cpp
 *
 *  Created on: May 31, 2011
 *      Author: stryder
 */

#include "Window.h"
#include "GameData.h"
#include "Camera.h"

Window::Window()
{
	const SDL_VideoInfo* screenResolution = SDL_GetVideoInfo();

	//Screen Attributes (camera size)
	SCREEN_WIDTH = screenResolution->current_w;
	SCREEN_HEIGHT = screenResolution->current_h;

	//Set up the screen
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF );

	//If there's and error
	if( screen == NULL )
	{
		windowOK = false;
		return;
	}
	else
	{
		windowOK = true;
	}

	//Set the window caption
	SDL_WM_SetCaption( "Space Tower Defence", NULL );

	//Set window flag
	windowed = true;

	LEVEL_WIDTH = 2500;
	LEVEL_HEIGHT = 1406;

	myCamera = new Camera( this );
}

Window::~Window()
{
	delete( myCamera );
}

void Window::toggle_fullscreen()
{
	//If the screen is windowed
	if( windowed == true )
	{
		//Set the screen to full screen
		screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF | SDL_FULLSCREEN );

		//If there's an error
		if( screen == NULL )
		{
			windowOK = false;
			return;
		}

		SDL_Flip( screen );

		//Set the window state flag
		windowed = false;
	}
	//If the screen is fullscreen
	else if( windowed == false )
	{
		//Window the screen
		screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF );

		//If there's an error
		if( screen == NULL )
		{
			windowOK = false;
			return;
		}

		//Set the window state flag
		windowed = true;
	}
}

void Window::handle_events( GameData* game_data )
{
	//If there's something wrong with the window
	if( windowOK == false )
	{
		return;
	}

	//If the window resized
	if ( game_data->get_EventStruc()->type == SDL_VIDEORESIZE )
	{
		//Resize the screen
		screen = SDL_SetVideoMode ( game_data->get_EventStruc()->resize.w, game_data->get_EventStruc()->resize.h, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF );

		//If there's an error
		if( screen == NULL )
		{
			windowOK = false;
			return;
		}
	}

	//If F5 was pressed
	else if( ( game_data->get_EventStruc()->type == SDL_KEYDOWN ) && ( game_data->get_EventStruc()->key.keysym.sym == SDLK_F5 ) )
	{
		//Turn full screen on/off
		toggle_fullscreen();
	}

	//If the window focus changed
	else if ( game_data->get_EventStruc()->type == SDL_ACTIVEEVENT )
	{
		//If the window was iconified or restored
		if( game_data->get_EventStruc()->active.state & SDL_APPACTIVE )
		{
			//If the application is no longer active
			if( game_data->get_EventStruc()->active.gain == 0 )
			{
				SDL_WM_SetCaption( "Window Event Test: Iconified", NULL);
			}
			else
			{
				SDL_WM_SetCaption( "Window Event Test", NULL );
			}
		}
		//If something happened to the kyboard focus
		else if( game_data->get_EventStruc()->active.state & SDL_APPINPUTFOCUS )
		{
			//If the application lost keyboard focus
			if( game_data->get_EventStruc()->active.gain == 0 )
			{
				SDL_WM_SetCaption( "Window Event Test: Keyboard focus lost", NULL );
			}
			else
			{
				SDL_WM_SetCaption( "Window Event Test", NULL );
			}
		}
		//If Something happened to the mouse focus
		else if( game_data->get_EventStruc()->active.state & SDL_APPMOUSEFOCUS )
		{
			//If the application lost mouse focus
			if( game_data->get_EventStruc()->active.gain == 0 )
			{
				SDL_WM_SetCaption( "Window Event Test: Mouse Focus Lost", NULL );
			}
			else
			{
				SDL_WM_SetCaption( "Window Event Test", NULL );
			}
		}
	}
	//If the window's screen has been altered
	else if( game_data->get_EventStruc()->type == SDL_VIDEOEXPOSE )
	{
		//Update the screen
		if( SDL_Flip( screen ) == -1 )
		{
			//If there's an error
			windowOK = false;
			return;
		}
	}
}

bool Window::error()
{
	return !windowOK;
}

int Window::get_ScreenWidth()
{
	return SCREEN_WIDTH;
}

int Window::get_ScreenHeight()
{
	return SCREEN_HEIGHT;
}

int Window::get_LevelWidth()
{
	return LEVEL_WIDTH;
}

int Window::get_LevelHeight()
{
	return LEVEL_HEIGHT;
}

int Window::get_ScreenVel()
{
	return SCREEN_VEL;
}

SDL_Surface* Window::get_screen()
{
	return screen;
}
Camera* Window::get_camera()
{
	return myCamera;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
	//Holds offsets
	SDL_Rect offset;

	//Get offsets
	offset.x = x;
	offset.y = y;

	//Blit
	SDL_BlitSurface( source, clip, destination, &offset );
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, Camera* camera )
{
	//Holds offsets
	SDL_Rect offset;

	SDL_Rect* clip = new SDL_Rect();

	clip->x = camera->getX();
	clip->y = camera->getY();
	clip->w = camera->getW();
	clip->h = camera->getH();

	//Get offsets
	offset.x = x;
	offset.y = y;

	//Blit
	SDL_BlitSurface( source, clip, destination, &offset );
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
	//Holds offsets
	SDL_Rect offset;

	SDL_Rect* clip = NULL;

	//Get offsets
	offset.x = x;
	offset.y = y;

	//Blit
	SDL_BlitSurface( source, clip, destination, &offset );
}

void apply_text_surface( int  x, int y, SDL_Surface* destination, const char* text, TTF_Font* font, SDL_Color textColour )
{
	SDL_Surface* Text = TTF_RenderText_Solid( font, text, textColour );
	apply_surface( x, y, Text, destination );
}
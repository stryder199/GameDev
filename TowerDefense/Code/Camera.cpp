/*
 * Camera.cpp
 *
 *  Created on: Jun 3, 2011
 *      Author: stryder
 */

#include "Camera.h"
#include "Window.h"
#include "GameData.h"

Camera::Camera( Window *myWindow )
{
	//Camera Box size
	x = 0;
	y = 0;
	w = myWindow->get_ScreenWidth();
	h = myWindow->get_ScreenHeight() - 150;

	//Initialize the velocity
	xVel = 0;
	yVel = 0;

	topScroll = false;
	botScroll = false;
	leftScroll = false;
	rightScroll = false;

}

void Camera::move(Uint32 deltaTicks, Window* myWindow )
{
	//Move the camera left or right
	x += (int)(xVel * ( deltaTicks / 1000.f ));

	//Keep the camera in bounds
	if( x < 0 )
	{
		x = 0;
	}
	else if( x > myWindow->get_LevelWidth() - w )
	{
		x = myWindow->get_LevelWidth() - w;
	}

	//Move the camera up or down
	y += (int)(yVel * ( deltaTicks / 1000.f ));

	//Keep the Camera in bounds
	if( y < 0 )
	{
		y = 0;
	}
	else if( y > myWindow->get_LevelHeight() - h )
	{
		y = myWindow->get_LevelHeight() - h;
	}
}

//Move to a set point on the screen, xCord and yCord
void Camera::moveTo( Window* myWindow, int xCord, int yCord )
{
	//Change the x,y cords of the Camera
	x = xCord;
	y = yCord;

	//Make sure you aren't moving to somewhere out of bounds
	if( x < 0 )
	{
		x = 0;
	}
	if( y < 0 )
	{
		y = 0;
	}
	if( x > myWindow->get_LevelWidth() - w )
	{
		x = myWindow->get_LevelWidth() - w;
	}
	if( y > myWindow->get_LevelHeight() - h )
	{
		y = myWindow->get_LevelHeight() - h;
	}
}

//Handles any keyboard or mouse input dealing with the Camera
void Camera::handle_input( Window* myWindow, GameData* game_data )
{
	int mouseX, mouseY;

	SDL_GetMouseState( &mouseX, &mouseY );

	//If a key was pressed
	if( game_data->get_EventStruc()->type == SDL_KEYDOWN )
	{
		//Adjust the velocity
		switch( game_data->get_EventStruc()->key.keysym.sym )
		{
		case SDLK_UP: yVel -= myWindow->get_ScreenVel(); break;
		case SDLK_DOWN: yVel += myWindow->get_ScreenVel(); break;
		case SDLK_LEFT: xVel -= myWindow->get_ScreenVel(); break;
		case SDLK_RIGHT: xVel += myWindow->get_ScreenVel(); break;
		default: break;
		}
	}
	//If a key was released
	else if( game_data->get_EventStruc()->type == SDL_KEYUP )
	{
		//Adjust the velocity back to what is was
		switch( game_data->get_EventStruc()->key.keysym.sym )
		{
		case SDLK_UP: yVel += myWindow->get_ScreenVel(); break;
		case SDLK_DOWN: yVel -= myWindow->get_ScreenVel(); break;
		case SDLK_LEFT: xVel += myWindow->get_ScreenVel(); break;
		case SDLK_RIGHT: xVel -= myWindow->get_ScreenVel(); break;
		default: break;
		}
	}
	else
	{

		if( mouseX >= game_data->get_rect("rightScreenScroll")->x && mouseX <= ( game_data->get_rect("rightScreenScroll")->x + game_data->get_rect("rightScreenScroll")->w) )
		{
			if( mouseY >= game_data->get_rect("rightScreenScroll")->y && mouseY <= ( game_data->get_rect("rightScreenScroll")->y + game_data->get_rect("rightScreenScroll")->h) && rightScroll == false )
			{
				xVel += myWindow->get_ScreenVel();
				rightScroll = true;
			}
		}
		else
		{
			if( rightScroll == true )
			{
				xVel -= myWindow->get_ScreenVel();
				rightScroll = false;
			}
		}

		if( mouseX >= game_data->get_rect("leftScreenScroll")->x && mouseX <= ( game_data->get_rect("leftScreenScroll")->x + game_data->get_rect("leftScreenScroll")->w) )
		{
			if( mouseY >= game_data->get_rect("leftScreenScroll")->y && mouseY <= ( game_data->get_rect("leftScreenScroll")->y + game_data->get_rect("leftScreenScroll")->h) && leftScroll == false )
			{
				xVel -= myWindow->get_ScreenVel();
				leftScroll = true;
			}
		}
		else
		{
			if( leftScroll == true )
			{
				xVel += myWindow->get_ScreenVel();
				leftScroll = false;
			}
		}

		if( mouseY >= game_data->get_rect("topScreenScroll")->y && mouseY <= ( game_data->get_rect("topScreenScroll")->y + game_data->get_rect("topScreenScroll")->h) )
		{
			if( mouseX >= game_data->get_rect("topScreenScroll")->x && mouseX <= ( game_data->get_rect("topScreenScroll")->x + game_data->get_rect("topScreenScroll")->w) && topScroll == false )
			{
				yVel -= myWindow->get_ScreenVel();
				topScroll = true;
			}
		}
		else
		{
			if( topScroll == true )
			{
				yVel += myWindow->get_ScreenVel();
				topScroll = false;
			}
		}

		if( mouseY >= game_data->get_rect("botScreenScroll")->y && mouseY <= ( game_data->get_rect("botScreenScroll")->y + game_data->get_rect("botScreenScroll")->h) )
		{
			if( mouseX >= game_data->get_rect("botScreenScroll")->x && mouseX <= ( game_data->get_rect("botScreenScroll")->x + game_data->get_rect("botScreenScroll")->w) && botScroll == false )
			{
				yVel += myWindow->get_ScreenVel();
				botScroll = true;
			}
		}
		else
		{
			if( botScroll == true )
			{
				yVel -= myWindow->get_ScreenVel();
				botScroll = false;
			}
		}
	}
}

//Returns the X coordinate of the Camera
int Camera::getX()
{
	return x;
}

//Returns the Y coordinate of the Camera
int Camera::getY()
{
	return y;
}

//Returns the Width of the Camera (should be constant)
int Camera::getW()
{
	return w;
}

//Returns the Height of the Camera (should be constant)
int Camera::getH()
{
	return h;
}

//Set the x coord to param var
void Camera::setX( int var )
{
	x = var;
}

//Set the y coord to param var
void Camera::setY( int var )
{
	y = var;
}

//Reset the Camera back to the initial conditions, usually done at the end of the game
void Camera::resetCamera()
{
	x = 0;
	y = 0;

	xVel = 0;
	yVel = 0;
}
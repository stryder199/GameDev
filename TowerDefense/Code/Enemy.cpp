/*
 * Dot.cpp
 *
 *  Created on: May 31, 2011
 *      Author: stryder
 */

#include "Enemy.h"
#include "Window.h"
#include "GameData.h"
#include "Camera.h"
#include "Timer.h"
#include "Resources.h"
#include "UI.h"

Enemy::Enemy( int id_number, int enemy_health, int velocity, int enemy_defense, Window* myWindow, GameData* game_data )
{
	//The timer that keeps an enemy slowed for 5 secs
	slowedTimer = new Timer();

	//Unique ID number
	ID = id_number;

	//Initialize offsets
	x = 0;
	y = myWindow->get_ScreenHeight()/3 + 300/2;

	//Initialize the velocity
	xVel = (float)velocity;
	yVel = (float)velocity;

	//Stored xVel,yVel so it can be restored if changed
	prev_xVel = xVel;
	prev_yVel = yVel;

	//If the enemy is selected
	selected = false;

	//Is the enemy being slowed by an Ice tower?
	slowed = false;

	//The current health of the enemy
	health = enemy_health;

	//The maximum health of the enemy
	maxhealth = enemy_health;

	//The attack damage of the enemy (not relavent right now)
	attack = 2;

	//The defense of the enemy (notrelavent right now)
	defense = enemy_defense;

	//initialize animation variable
	frame = 0;

	frame1 = game_data->get_surfaceArray("shipFrame1");
	frame2 = game_data->get_surfaceArray("shipFrame2");
	frame3 = game_data->get_surfaceArray("shipFrame3");
	frame4 = game_data->get_surfaceArray("shipFrame4");
	frame5 = game_data->get_surfaceArray("shipFrame5");
	frame6 = game_data->get_surfaceArray("shipFrame6");
}

//Move the Enemy on the screen relative to the frame rate
void Enemy::move( Uint32 deltaTicks, Window* myWindow )
{
	//Handle the case if unit is being slowed and its been slowed for 5 secs
	//So the slow needs to be removed
	if( slowed == true && slowedTimer->get_ticks() > 5000 )
	{
		slowed = false;
		slowedTimer->stop();
		xVel = prev_xVel;
		yVel = prev_yVel;
	}


	//Move the Enemy left or right
	x += (int)(xVel * ( deltaTicks / 2000.f ));

	//If the Enemy went too far to the left
	if( x < 0 )
	{
		//Move back
		x = 0;
	}
	//or the right
	else if( x + ENEMY_WIDTH > myWindow->get_LevelWidth() )
	{
		//Move back
		x = myWindow->get_LevelWidth() - ENEMY_WIDTH;
	}

	//Move the Enemy up or down
	y += (int)(yVel * ( deltaTicks / 1000.f ));

	//If the Enemy went too far up
	if( y < 0 )
	{
		//Move back
		yVel = yVel*-1;
		prev_yVel = yVel;
	}
	//or down
	else if( y + ENEMY_HEIGHT > myWindow->get_LevelHeight() )
	{
		//Move back
		yVel = yVel*-1;
		prev_yVel = yVel;
	}
}

//Handles the Enemy bouncing around the menu Screen, same move bounds dont apply as before
void Enemy::menuMove( Uint32 deltaTicks, Window* myWindow )
{
	//Move the Enemy left or right
	x += (int)(xVel * ( deltaTicks / 2000.f ));

	//If the Enemy went too far to the left
	if( x < 0 )
	{
		//Move back
		xVel = xVel*-1;
	}
	//or the right
	else if( x + ENEMY_WIDTH > myWindow->get_ScreenWidth() )
	{
		//Move back
		xVel = xVel*-1;
	}

	//Move the Enemy up or down
	y += (int)(yVel * ( deltaTicks / 1000.f ));

	//If the Enemy went too far up
	if( y < 0 )
	{
		//Move back
		yVel = yVel*-1;
	}
	//or down
	else if( y + ENEMY_HEIGHT > myWindow->get_ScreenHeight() )
	{
		//Move back
		yVel = yVel*-1;
	}
}

//The camera will follow this enemy as it moves across the screen
void Enemy::set_camera( Window* myWindow )
{
	//Center the camera over the Enemy
	myWindow->get_camera()->setX(( x+ENEMY_WIDTH /2 ) - myWindow->get_ScreenWidth()/2);
	myWindow->get_camera()->setY(( y+ENEMY_HEIGHT /2 ) - myWindow->get_ScreenHeight()/2);

	//Keep the camera in bounds
	if( myWindow->get_camera()->getX() < 0 )
	{
		myWindow->get_camera()->setX( 0 );
	}
	if( myWindow->get_camera()->getY() < 0 )
	{
		myWindow->get_camera()->setY( 0 );
	}
	if( myWindow->get_camera()->getX() > myWindow->get_LevelWidth() - myWindow->get_camera()->getW() )
	{
		myWindow->get_camera()->setX( myWindow->get_LevelWidth() - myWindow->get_camera()->getW() );
	}
	if( myWindow->get_camera()->getY() > myWindow->get_LevelHeight() - myWindow->get_camera()->getH() )
	{
		myWindow->get_camera()->setY( myWindow->get_LevelHeight() - myWindow->get_camera()->getH() );
	}
}

void Enemy::show( Window* myWindow, GameData* game_data )
{
	SDL_Surface *currentImage = NULL;

	//Move next frame in animation
	frame++;

	if(frame >=6)
		frame=0;

	//The health bars above the enemy
	SDL_Rect HealthBarRed;
	SDL_Rect HealthBarGreen;
	
	//Sets it above the enemy and the red bar is of constant size
	HealthBarRed.x = x + 10 - myWindow->get_camera()->getX();
	HealthBarRed.y = y - 10 - myWindow->get_camera()->getY();
	HealthBarRed.w = 40;
	HealthBarRed.h = 3;

	//Overlays a green bar over the red bar but its width changes as you lose health
	HealthBarGreen.x = HealthBarRed.x;
	HealthBarGreen.y = HealthBarRed.y;
	HealthBarGreen.w = (HealthBarRed.w*health)/maxhealth;
	HealthBarGreen.h = HealthBarRed.h;

	//Make sure if your green health bar doesnt go crazy if health is negative for some reason
	if( HealthBarGreen.w > HealthBarRed.w )
	{
		HealthBarGreen.w = 0;
	}

	float angle = enemyAngle();

	switch(frame)
	{
	case 0:	currentImage = getEnemyRotatedImage( int(angle), frame1 ); break;
	case 1: currentImage = getEnemyRotatedImage( int(angle), frame2 ); break;
	case 2: currentImage = getEnemyRotatedImage( int(angle), frame3 ); break;
	case 3: currentImage = getEnemyRotatedImage( int(angle), frame4 ); break;
	case 4: currentImage = getEnemyRotatedImage( int(angle), frame5 ); break;
	case 5: currentImage = getEnemyRotatedImage( int(angle), frame6 ); break;
	default: currentImage = getEnemyRotatedImage( int(angle), frame1 ); break;
	}

	//Applies the enemy on the screen
	apply_surface((int)x - myWindow->get_camera()->getX(), (int)y - myWindow->get_camera()->getY(), currentImage, myWindow->get_screen() );

	//Applies the green and red healthbar's on the screen
	SDL_FillRect( myWindow->get_screen(), &HealthBarRed, SDL_MapRGB( myWindow->get_screen()->format, 0xFF, 0x24, 0x00 ) );
	SDL_FillRect( myWindow->get_screen(), &HealthBarGreen, SDL_MapRGB( myWindow->get_screen()->format, 0x09, 0xF9, 0x11 ) );


	//If the enemy is slowed put a light blue circle over it (change to a mask on the surface to get rid of the clipping)
	if( slowed == true )
	{
		filledCircleRGBA( myWindow->get_screen(), Sint16(x) - myWindow->get_camera()->getX() + ENEMY_WIDTH/2, Sint16(y) - myWindow->get_camera()->getY() + ENEMY_HEIGHT/2, Sint16(ENEMY_WIDTH/2), 0x00, 0xff, 0xff, Uint8(0x55));
	}

}

bool Enemy::isSelected()
{
	return selected;
}

void Enemy::setX( int val )
{
	x = val;
}

void Enemy::offSetY( int val )
{
	y = y + val;
}

void Enemy::setY( int val )
{
	y = val;
}

int Enemy::getX()
{
	return x;
}

int Enemy::getEnemyNum()
{
	return ID;
}

int Enemy::getY()
{
	return y;
}

int Enemy::getHealth()
{
	return health;
}

void Enemy::offsetHealth( int x )
{
	 health = health + x;
}

int Enemy::getMaxHealth()
{
	return maxhealth;
}

//It just works, don't try and understand why
float Enemy::enemyAngle()
{
	if( xVel <= 0 )
	{
		if( yVel <= 0 )
		{
			return ( atan2( (float)(fabs(xVel)), (float)(fabs(yVel)) )* 180.0f / (atan(1.0f)*4)) + 90.0f ;
		}
		if( yVel > 0 )
		{
			return ( atan2( (float)(fabs(xVel)), (float)(fabs(yVel)) )* 180.0f / (atan(1.0f)*4.0f)) + 180.0f;
		}
	}
	if( xVel > 0 )
	{
		if( yVel <= 0  )
		{
			return ( atan2( (float)(fabs(xVel)), (float)(fabs(yVel)) )* 180.0f / (atan(1.0f)*4.0f));
		}
		if( yVel > 0 )
		{
			return ( atan2( (float)(fabs(xVel)), (float)(fabs(yVel)) )* 180.0f / (atan(1.0f)*4.0f)) + 270.0f;
		}
	}
	//should never reach
	return 0;
}

SDL_Surface* getEnemyRotatedImage( int itsOrientation, SDL_Surface* image[] )
{
	//return the floor of range(0-360)/2
	int array_position = itsOrientation/2;

	return image[array_position];
}

void Enemy::setSlowed()
{
	if( slowed == false )
	{
		
		xVel -= ((xVel)/3);
		
		yVel -= ((yVel)/3);
		
	}
	slowed = true;
	slowedTimer->start();
}

bool Enemy::getSlowed()
{
	return slowed;
}

void applyEnemies( GameData *game_data , Resources* myResources, Timer *delta, int *enemy_counter, UI* myUI, Window *myWindow )
{
	std::vector<Enemy*> deadEnemies;
	std::list<Enemy*>::iterator it;
	std::vector<Enemy*>::iterator it2;

	//Show the all Enemies on the screen
	for( it=game_data->get_AllEnemies()->begin(); it != game_data->get_AllEnemies()->end(); it++)
	{
		if(*it != NULL)
		{
			(*it)->show( myWindow , game_data );
			(*it)->move( delta->get_ticks(), myWindow );

			if( (*it)->getX() > (myWindow->get_LevelWidth() - 2*Enemy::ENEMY_WIDTH) || (*it)->getHealth() <= 0)
			{
				//Add the off screen Enemies to deadEnemies
				deadEnemies.push_back(*it);
				enemy_counter--;
				if( (*it) == myUI->getEnemyFocus() )
				{
					myUI->nullFocus();
				}
			}
			if( (*it)->getHealth() <= 0 )
			{
				myResources->add_score( 10 );
				myResources->add_money( myResources->get_wave_number() );
			}
			if( (*it)->getX() > myWindow->get_LevelWidth() - 2*Enemy::ENEMY_WIDTH )
			{
				myResources->add_lives( -1 );
			}
		}
	}

	//Clean up dead Enemies
	for( it2 = deadEnemies.begin(); it2 != deadEnemies.end(); it2++)
	{
		if(*it2 != 0x0)
		{
			game_data->get_AllEnemies()->remove(*it2);
		}
	}
	deadEnemies.clear();
}

Enemy* spawnEnemy( GameData *game_data, int &enemy_counter, int &ID_number, int health, int velocity, int defense, Window *myWindow )
{
	Enemy* newEnemy = new Enemy( ID_number, health, velocity, defense, myWindow, game_data );

	game_data->get_AllEnemies()->push_back(newEnemy);

	enemy_counter++;
	ID_number++;

	return newEnemy;
}
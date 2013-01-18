/*
 * Tower.cpp
 *
 *  Created on: Oct 6, 2011
 *      Author: stryder
 */

#include "Tower.h"
#include "Enemy.h"
#include "Window.h"
#include "GameData.h"
#include "UI.h"
#include "Camera.h"

//Case1 = basicCannon
//Case2 = basicTank
//Case3 = iceTower
//Case4 = nukeTower
//Case5 = laserTower

//Upgrade tiers, basic A and B, medium if A- C D if B- E F, 10,20,30 = evolve
//No UG = 1
//Basic = 1-10                                                    ->C ->C (Dmg)
//A = 11-20                            ->A ->A (Dmg)    ---------
//B = 21-30                                                       ->D ->D (range)
//C = 31-40         0 -> Basic -------
//D = 41-50                                                       ->E ->E (speed)
//E = 51-60                            ->B ->B (speed)  ---------
//F = 61-70                                                       ->F ->F (range)


Tower::Tower( int type_number, Window* myWindow, GameData* game_data )
{
	assert( (type_number < 6) && (type_number >= 0) );

	onTopTower = false;
	attackTimer = new Timer();

	x = 0 + myWindow->get_camera()->getX();
	y = 0 + myWindow->get_camera()->getY();
	type = type_number;
	upgrade_teir = 1;
	target = NULL;
	laserWidth = 0x05;
	alive = true;

	switch (type){
	case 1:
		title = "Cannon Tower";
		description = "Ground Enemies -- Single Target";
		image_path = game_data->get_surface( "cannonTower" );
		damage = 5;
		attackspeed = 600;
		slow = 0;
		range = 500;
		sell_return = 100;
		buy_cost = 100;
		laserColorR = 0x7c;
		laserColorG = 0xfc;
		laserColorB = 0x00;
		laserWidth = 0x5;
		icon = game_data->get_surface( "cannonTowerSmall" );
		rotationArray = game_data->get_surfaceArray( "cannonImages" );
		break;
	case 2:
		title = "Tank Tower";
		description = "Ground Enemies -- AOE Damage";
		image_path = game_data->get_surface( "tankTower" );
		damage = 50;
		attackspeed = 300;
		slow = 0;
		range = 500;
		sell_return = 100;
		buy_cost = 150;
		laserColorR = 0xff;
		laserColorG = 0xff;
		laserColorB = 0x00;
		laserWidth = 0x5;
		icon = game_data->get_surface( "tankTowerSmall" );
		rotationArray = game_data->get_surfaceArray( "tankImages" );
		break;
	case 3:
		title = "Ice Tower";
		description = "Ground Enemies -- Slows Targets";;
		image_path = game_data->get_surface( "iceTower" );
		damage = 5;
		attackspeed = 300;
		slow = 0;
		range = 500;
		sell_return = 100;
		buy_cost = 200;
		laserColorR = 0x7f;
		laserColorG = 0xff;
		laserColorB = 0xd4;
		laserWidth = 0x5;
		icon = game_data->get_surface( "iceTowerSmall" );
		rotationArray = game_data->get_surfaceArray( "iceImages" );
		break;
	case 4:
		title = "Nuke Tower";
		description = "Ground/Air -- High Damage/Slow Reload";
		image_path = game_data->get_surface( "nukeTower" );
		damage = 75;
		attackspeed = 1000;
		slow = 10;
		range = 500;
		sell_return = 100;
		buy_cost = 300;
		laserColorR = 0x94;
		laserColorG = 0x00;
		laserColorB = 0xd3;
		laserWidth = 0x5;
		icon = game_data->get_surface( "nukeTowerSmall" );
		rotationArray = game_data->get_surfaceArray( "nukeImages" );
		break;
	case 5:
		title = "Laser Tower";
		description = "Ground/Air Enemies -- Constant Damage";
		image_path = game_data->get_surface( "laserTower" );
		damage = 1;
		attackspeed = 10;
		slow = 0;
		range = 500;
		sell_return = 100;
		buy_cost = 300;
		laserColorR = 0xff;
		laserColorG = 0x00;
		laserColorB = 0x00;
		laserWidth = 0x5;
		icon = game_data->get_surface( "laserTowerSmall" );
		rotationArray = game_data->get_surfaceArray( "laserImages" );
		break;
	default:
		break;
	}

	//Copy the tower image and create a new tower image
	image = SDL_ConvertSurface(image_path, image_path->format, SDL_SWSURFACE);
	sound = game_data->get_sound( "laserSound3" );
	lastImg = image;
	planted = false;
	attackTimer->start();
}

//Menu Towers
Tower::Tower( SDL_Surface *image_path , int type_number, int xvar, int yvar, Mix_Chunk *sound_path, Window* myWindow, GameData* game_data )
{
	attackTimer = new Timer();

	x = xvar;
	y = yvar;
	type = type_number;
	upgrade_teir = 1;
	target = NULL;
	laserWidth = 0x05;
	alive = true;
	planted = true;

	switch (type){
	case 1:
		title = "Cannon Tower";
		description = "Ground Enemies -- Single Target";
		damage = 10;
		attackspeed = 300;
		slow = 0;
		range = 500;
		sell_return = 100;
		buy_cost = 100;
		laserColorR = 0x7c;
		laserColorG = 0xfc;
		laserColorB = 0x00;
		laserWidth = 0x5;
		icon = game_data->get_surface( "cannonTowerSmall" );
		rotationArray = game_data->get_surfaceArray( "cannonImages" );
		break;
	case 2:
		title = "Tank Tower";
		description = "Ground Enemies -- AOE Damage";
		damage = 50;
		attackspeed = 300;
		slow = 0;
		range = 500;
		sell_return = 100;
		buy_cost = 100;
		laserColorR = 0xff;
		laserColorG = 0xff;
		laserColorB = 0x00;
		laserWidth = 0x5;
		icon = game_data->get_surface( "tankTowerSmall" );
		rotationArray = game_data->get_surfaceArray( "tankImages" );
		break;
	case 3:
		title = "Ice Tower";
		description = "Ground Enemies -- Slows Targets";;
		damage = 1;
		attackspeed = 300;
		slow = 0;
		range = 500;
		sell_return = 100;
		buy_cost = 100;
		laserColorR = 0x7f;
		laserColorG = 0xff;
		laserColorB = 0xd4;
		laserWidth = 0x5;
		icon = game_data->get_surface( "iceTowerSmall" );
		rotationArray = game_data->get_surfaceArray( "iceImages" );
		break;
	case 4:
		title = "Nuke Tower";
		description = "Ground/Air -- High Damage/Slow Reload";
		damage = 75;
		attackspeed = 300;
		slow = 10;
		range = 500;
		sell_return = 100;
		buy_cost = 100;
		laserColorR = 0x94;
		laserColorG = 0x00;
		laserColorB = 0xd3;
		laserWidth = 0x5;
		icon = game_data->get_surface( "nukeTowerSmall" );
		rotationArray = game_data->get_surfaceArray( "nukeImages" );
		break;
	case 5:
		title = "Laser Tower";
		description = "Ground/Air Enemies -- Constant Damage";
		damage = 1;
		attackspeed = 10;
		slow = 0;
		range = 500;
		sell_return = 100;
		buy_cost = 100;
		laserColorR = 0xff;
		laserColorG = 0x00;
		laserColorB = 0x00;
		laserWidth = 0x5;
		icon = game_data->get_surface( "laserTowerSmall" );
		rotationArray = game_data->get_surfaceArray( "laserImages" );
		break;
	default:
		break;
	}

	//Copy the tower image and create a new tower image
	image = SDL_ConvertSurface(image_path, image_path->format, SDL_SWSURFACE);
	sound = sound_path;
	lastImg = image;
	attackTimer->start();
}

Tower::~Tower()
{

}

void Tower::handleAttack( Window* myWindow )
{
	if( (target != NULL) && (planted == true) )
	{
		if( target->getHealth() <= 0 || target->getX() > (myWindow->get_LevelWidth()-10))
		{
			target = NULL;
		}
	}

	if( attackspeed <= attackTimer->get_ticks() )
	{
		if( target != NULL )
		{
			Mix_PlayChannel( -1, sound, 0 );
			target->offsetHealth( -1*damage );
			if( x > target->getX() )
			{
				thickLineRGBA( myWindow->get_screen(), target->getX() + 40/2 - myWindow->get_camera()->getX(), target->getY() + 40/2 - myWindow->get_camera()->getY(),  x - myWindow->get_camera()->getX(), y - myWindow->get_camera()->getY(), laserWidth, laserColorR, laserColorG, laserColorB, 0xFF );
			}
			else
			{
				thickLineRGBA( myWindow->get_screen(), x - myWindow->get_camera()->getX(), y - myWindow->get_camera()->getY(), target->getX() + 40/2 - myWindow->get_camera()->getX(), target->getY() + 40/2 - myWindow->get_camera()->getY(), laserWidth, laserColorR, laserColorG, laserColorB, 0xFF );
			}
			if( type == 3 )
			{
				target->setSlowed();
			}
		}
		attackTimer->start();
	}
}

void Tower::show( Window* myWindow, GameData* game_data, UI* myUI )
{
	if(planted == true)
	{
		//Show the unit at the center of the image
		if( target != NULL )
		{
			double angle = towerAngle( this, target );
			SDL_Surface *currentImage = getRotatedImage( int(angle), rotationArray );
			apply_surface( (int)x - currentImage->w/2 - myWindow->get_camera()->getX(), (int)y - currentImage->h/2 - myWindow->get_camera()->getY(), currentImage, myWindow->get_screen() );
			lastImg = currentImage;
		}
		else
		{
			apply_surface( (int)x - lastImg->w/2 - myWindow->get_camera()->getX(), (int)y - lastImg->h/2 - myWindow->get_camera()->getY(), lastImg, myWindow->get_screen() );
		}

		if( myUI->getTowFocus() == this )
		{
			circleRGBA( myWindow->get_screen(), Sint16(x) + TOWER_WIDTH/2 - myWindow->get_camera()->getX(), Sint16(y) + TOWER_HEIGHT/2 - myWindow->get_camera()->getY(), Sint16(range), Uint8(0xFF), Uint8(0xFF), Uint8(0xFF), Uint8(0xFF));
		}
	}
	//else planted == false and follow the mouse
	else
	{
		SDL_GetMouseState(&x, &y);
		x+= myWindow->get_camera()->getX();
		y+= myWindow->get_camera()->getY();

		apply_surface( (int)x - lastImg->w/2 - myWindow->get_camera()->getX(), (int)y - lastImg->h/2 - myWindow->get_camera()->getY(), lastImg, myWindow->get_screen() );

		
		
		//Green if nothing Blocking
		if( onTopTower == false )
		{
			//Range circle
			circleRGBA( myWindow->get_screen(), Sint16(x) + TOWER_WIDTH/2 - myWindow->get_camera()->getX(), Sint16(y) + TOWER_HEIGHT/2 - myWindow->get_camera()->getY(), Sint16(range), Uint8(0x09), Uint8(0xF9), Uint8(0x11), Uint8(0xFF));

			//Circle over Tower
			filledCircleRGBA( myWindow->get_screen(), Sint16(x) - myWindow->get_camera()->getX(), Sint16(y) - myWindow->get_camera()->getY(), Sint16(TOWER_WIDTH/2), Uint8(0x09), Uint8(0xF9), Uint8(0x11), Uint8(0x55));
		}
		//Red if you cant Plant
		else
		{
			//Range circle
			circleRGBA( myWindow->get_screen(), Sint16(x) + TOWER_WIDTH/2 - myWindow->get_camera()->getX(), Sint16(y) + TOWER_HEIGHT/2 - myWindow->get_camera()->getY(), Sint16(range), Uint8(0xFF), Uint8(0x24), Uint8(0x00), Uint8(0xFF));

			//Circle over Tower
			filledCircleRGBA( myWindow->get_screen(), Sint16(x) - myWindow->get_camera()->getX(), Sint16(y) - myWindow->get_camera()->getY(), Sint16(TOWER_WIDTH/2), Uint8(0xFF), Uint8(0x24), Uint8(0x00), Uint8(0x55));
		}
	}
}

//Menu show!
void Tower::show( SDL_Surface *rotatedImage, Window* myWindow )
{
	//Show the unit at the center of the image
	apply_surface( (int)x - rotatedImage->w/2 - myWindow->get_camera()->getX(), (int)y - rotatedImage->h/2 - myWindow->get_camera()->getY(), rotatedImage, myWindow->get_screen() );
}

bool Tower::getPlanted()
{
	return planted;
}

void Tower::setPlanted( bool x )
{
	planted = x;
}

void Tower::setX( int val )
{
	x = val;
}

void Tower::setY( int val )
{
	y = val;
}

int Tower::getX()
{
	return x;
}

int Tower::getY()
{
	return y;
}

int Tower::getX_cam( Window* myWindow )
{
	return(x - myWindow->get_camera()->getX());
}

int Tower::getY_cam( Window* myWindow )
{
	return(y - myWindow->get_camera()->getY());
}


Enemy* Tower::getTarget()
{
	return target;
}

void Tower::setTarget( Enemy* val )
{
	target = val;
}

int Tower::getRange()
{
	return range;
}

int Tower::getType()
{
	return type;
}

int Tower::getDamage()
{
	return damage;
}

int Tower::getAttackSpeed()
{
	return attackspeed;
}

int Tower::getBuyCost()
{
	return buy_cost;
}

int Tower::getSellCost()
{
	return sell_return;
}

SDL_Surface* Tower::getLastImg()
{
	return lastImg;
}

void Tower::setLastImage( SDL_Surface* img )
{
	lastImg = img;
}

int Tower::getUpgradeTeir()
{
	return upgrade_teir;
}

void Tower::setUpgradeTeir( int i )
{
	upgrade_teir = i;
}

void Tower::upgradeTeirPP()
{
	upgrade_teir++;
}

std::string Tower::getTitle()
{
	return title;
}

std::string Tower::getDescription()
{
	return description;
}

SDL_Surface* Tower::getIcon()
{
	return icon;
}

bool Tower::isAlive()
{
	return alive;
}

void Tower::setDead()
{
	alive = false;
}

void Tower::setTowOntop( bool var )
{
	onTopTower = var;
}

bool Tower::getTowOntop()
{
	return onTopTower;
}

//Needs some TLC
void Tower::handleUpgrades()
{
	//A annd B teir
	if( upgrade_teir == 11 )
	{
		//Increased Damage
		laserWidth += 2;
		damage += 20;
		sell_return += 10;
	}
	else if( upgrade_teir == 21 )
	{
		//Increased Attack Speed
		laserWidth += 2;
		attackspeed += 50;
		sell_return += 10;
	}

	//C and D teir
	else if( upgrade_teir == 31 )
	{
		//Increased Damage
		laserWidth += 2;
		damage += 30;
		sell_return += 10;
	}
	//End of upgrade borders
	else if( upgrade_teir == 41 )
	{
		//Increased Range
		laserWidth += 2;
		range += 100;
		damage += 10;
		attackspeed += 1;
		sell_return += 10;
	}


	//E and F teir
	else if( upgrade_teir == 51 )
	{
		//Increased Attack Speed
		laserWidth += 2;
		attackspeed += 100;
		sell_return += 10;
	}
	else if( upgrade_teir == 61 )
	{
		//Increased Range
		laserWidth += 2;
		range += 100;
		attackspeed += 50;
		sell_return += 10;
	}
	else
	{
		laserWidth += 1;
		range += 5;
		damage += 10;
		attackspeed += 1;
		sell_return += 5;
	}
}

void Tower::setRange( int var	)
{
	range = var;
}

//It just works, don't try and understand why
float towerAngle( Tower* tower, Enemy* enemy )
{
	if( tower->getX() <= enemy->getX() )
	{
		if( tower->getY() <= enemy->getY() )
		{
			return ( atan2((float)(enemy->getX() - tower->getX()), (float)(enemy->getY() - tower->getY()))* 180.0f / (atan(1.0f)*4)) + 270.0f;
		}
		if( tower->getY() > enemy->getY() )
		{
			return -1.0f*( atan2((float)(enemy->getX() - tower->getX()), (float)(tower->getY() - enemy->getY()))* 180.0f / (atan(1.0f)*4.0f)) + 90.0f;
		}
	}
	if( tower->getX() >= enemy->getX() )
	{
		if( tower->getY() <= enemy->getY() )
		{
			return -1.0f*(atan2((float)(tower->getX() - enemy->getX()), (float)(enemy->getY() - tower->getY()))* 180.0f / (atan(1.0f)*4.0f)) + 270.0f;
		}
		if( tower->getY() > enemy->getY() )
		{
			return (atan2((float)(tower->getX() - enemy->getX()), (float)(tower->getY() - enemy->getY()))* 180.0f / (atan(1.0f)*4.0f)) + 90.0f;
		}
	}
	//should never reach
	return 0;
}

//Possible bug at 360, no array position 180 :(
SDL_Surface* getRotatedImage( int itsOrientation, SDL_Surface* image[] )
{
	//return the floor of range(0-360)/2
	int array_position = itsOrientation/2;

	return image[array_position];
}

void applyTowers( Window *myWindow, GameData *game_data, UI* myUI )
{
	std::vector<Tower*> deadTowers;
	std::list<Tower*>::iterator it;
	std::vector<Tower*>::iterator it2;

	//Show the all Enemies on the screen
	for( it=game_data->get_AllTowers()->begin(); it != game_data->get_AllTowers()->end(); it++)
	{
		if((*it) != NULL)
		{
			(*it)->show( myWindow, game_data, myUI );
		}

		if( (*it)->isAlive() == false )
		{
			deadTowers.push_back(*it);
			if( (*it) == myUI->getTowFocus() )
			{
				myUI->nullFocus();
			}
		}
	}

	//Clean up dead Enemies
	for( it2 = deadTowers.begin(); it2 != deadTowers.end(); it2++)
	{
		if(*it2 != 0x0)
		{
			game_data->get_AllTowers()->remove(*it2);
		}
	}
	deadTowers.clear();
}
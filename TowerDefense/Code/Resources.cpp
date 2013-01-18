/*
 * Resources.cpp
 *
 *  Created on: Jun 2, 2011
 *      Author: stryder
 */

#include "Resources.h"

//Initalize the resources at the start of the game
Resources::Resources()
{
	money = 2000;
	score = 0;
	lives = 20;
	wave_number = 1;
}

//Returns how much money you have
int Resources::get_money()
{
	return money;
}

//Returns your current score
int Resources::get_score()
{
	return score;
}

//Returns the amount of lives remaining
int Resources::get_lives()
{
	return lives;
}

//Adds money as you kill an enemy or param x negative when you want to take money away
void Resources::add_money( int x )
{
	money += x;

	assert( money >= 0 );
}

//Adds score as you kill enemies
void Resources::add_score( int x )
{
	score += x;
}

//Subtracts lives as you lose, negative param x
void Resources::add_lives( int x )
{
	lives += x;
}

//Returns the current wave number that your on
int Resources::get_wave_number()
{
	return wave_number;
}

//When a new wave starts add 1 to wave number
void Resources::increment_wave_number()
{
	wave_number++;
}

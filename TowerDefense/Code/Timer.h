/*
 * Timer.h
 *
 *  Created on: May 31, 2011
 *      Author: stryder
 */

//=================================
// include guard
#ifndef TIMER_H_
#define TIMER_H_

//=================================
// forward declared dependencies

//=================================
// included dependencies
#ifdef __GNUC__ 	//If compiler cygwin/gcc on Windows XP and for gcc 32-bit and 64-bit Linux
#include "SDL/SDL.h"
#else 				//If Borland and Microsoft compilers
#include "SDL.h"
#endif

//The timer
class Timer
{
private:
	//The clock time when the timer started
	int startTicks;

	//The ticks stored when the timer was paused
	int pausedTicks;

	//The timer status
	bool paused;
	bool started;

public:
	//Initializes variables
	Timer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	int get_ticks();

	//Checks the status of the timer
	bool is_started();
	bool is_paused();
};

#endif /* TIMER_H_ */

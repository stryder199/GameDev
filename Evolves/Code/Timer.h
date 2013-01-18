//=================================
// include guard
#ifndef Timer_H_
#define Timer_H_

//=================================
// forward declared dependencies


//=================================
// included dependencies
#include <Windows.h>

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
	void restart();

	//Gets the timer's time
	int get_ticks();

	//Checks the status of the timer
	bool is_started();
	bool is_paused();
};


#endif /* Timer_H_ */
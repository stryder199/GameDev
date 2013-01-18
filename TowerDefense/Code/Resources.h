/*
 * Resources.h
 *
 *  Created on: Jun 2, 2011
 *      Author: stryder
 */

//=================================
// include guard
#ifndef RESOURCES_H_
#define RESOURCES_H_

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <cassert>

class Resources
{
private:

	int money, score, lives, wave_number;

public:

	Resources();
	int get_money();
	int get_score();
	int get_lives();
	void add_money( int x );
	void add_score( int x );
	void add_lives( int x );
	int get_wave_number();
	void increment_wave_number();
};


#endif /* RESOURCES_H_ */

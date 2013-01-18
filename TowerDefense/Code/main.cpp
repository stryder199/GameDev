/******************************************************************************************************************
 ******************************************************************************************************************
 **	Tower Defense Game, Written using SDL
 **
 **	Program starts at main - loads startup files, and controls the changing from menu to game, or game to menu
 **
 **	menu - As the name implies it is the main loop that shows the menu surfaces on the screen
 **
 **	mainGame - Where all the game function calls live, also the main loop that updates the screen at a certain fps,
 **			  the game loops in the main while loop until you lose or you quit the game
 **
 ******************************************************************************************************************
 ******************************************************************************************************************
 **/

//=================================
//	SDL Headers
#ifdef __GNUC__ 	//If compiler cygwin/gcc on Windows XP and for gcc 32-bit and 64-bit Linux
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#else 				//If Borland and Microsoft compilers
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_gfxPrimitives.h"
#include "SDL_rotozoom.h"
#endif

//=================================
//	class Headers
#include "Timer.h"
#include "Window.h"
#include "Resources.h"
#include "Camera.h"
#include "Enemy.h"
#include "Tower.h"
#include "GameData.h"
#include "myButton.h"
#include "MiniMap.h"
#include "UI.h"

//=================================
//	C++ Headers
#include <string>
#include <cassert>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

//=================================
//	Defines

//Comment out for no debug

#define DEBUG

//=================================
//	Function Forward Declaration

//=================================
//	Debug stuff
#ifdef DEBUG
std::ofstream debugf;
#endif

bool init()
{
	//Initialize all SDL subsystems
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		return false;
	}

	//Initialize SDL_ttf
	if( TTF_Init() == -1 )
	{
		return false;
	}

	//If everything initialized fine
	return true;
}

void waveSpawner( GameData *game_data, int &wave_size, int &enemy_counter, int &ID_number, Resources *myResources, Timer *spawnTimer, Window *myWindow )
{
	if( spawnTimer->get_ticks() > 1000)
	{
		if(wave_size < 10)
		{
			int enemy_health = int(10.0*exp((float)(myResources->get_wave_number())/4.0));

			spawnEnemy( game_data, enemy_counter, ID_number, enemy_health, 200, 1, myWindow );
			wave_size ++;
			spawnTimer->start();
		}
		else
		{
			if( spawnTimer->get_ticks() > 8000 )
			{
				wave_size = 0;
				myResources->increment_wave_number();
				spawnTimer->start();
			}
		}
	}
}

float pytha( float x, float y )
{
	float xsqur = x*x;
	float ysqur = y*y;
	float c = sqrt( xsqur + ysqur );
	return c;
}

float targetDistance( Tower* tower, Enemy* enemy, Window *myWindow )
{
	if( xRelCam( tower->getX(), myWindow ) <= xRelCam( enemy->getX(), myWindow ) )
	{
		if( yRelCam( tower->getY(), myWindow ) < yRelCam( enemy->getY(), myWindow ) )
		{
			return pytha( (float)( xRelCam( enemy->getX(), myWindow ) - xRelCam( tower->getX(), myWindow )), (float)( yRelCam( enemy->getY(), myWindow ) - yRelCam( tower->getY(), myWindow ) ));
		}
		if( yRelCam( tower->getY(), myWindow ) > yRelCam( enemy->getY(), myWindow ) )
		{
			return pytha( (float)(xRelCam( enemy->getX(), myWindow ) - xRelCam( tower->getX(), myWindow )), (float)( yRelCam( tower->getY(), myWindow ) - yRelCam( enemy->getY(), myWindow )));
		}
	}
	if( xRelCam( tower->getX(), myWindow ) >= xRelCam( enemy->getX(), myWindow ) )
	{
		if( yRelCam( tower->getY(), myWindow ) < yRelCam( enemy->getY(), myWindow ) )
		{
			return pytha( (float)(xRelCam( tower->getX(), myWindow ) - xRelCam( enemy->getX(), myWindow )), (float)( yRelCam( enemy->getY(), myWindow ) - yRelCam( tower->getY(), myWindow ) ));
		}
		if( yRelCam( tower->getY(), myWindow ) > yRelCam( enemy->getY(), myWindow ) )
		{
			return pytha( (float)(xRelCam( tower->getX(), myWindow ) - xRelCam( enemy->getX(), myWindow )), (float)( yRelCam( tower->getY(), myWindow ) - yRelCam( enemy->getY(), myWindow ) ));
		}
	}
	//should never reach
	return 0;
}

//Possible bug at 360, no array position 180 :(
bool check_collision( SDL_Rect* A, SDL_Rect* B )
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = A->x;
	rightA = A->x + A->w;
	topA = A->y;
	bottomA = A->y + A->h;

	//Calculate the sides of rect B
	leftB = B->x;
	rightB = B->x + B->w;
	topB = B->y;
	bottomB = B->y + B->h;

	//If any of the sides from A are outside of B
	if( bottomA <= topB )
	{
		return false;
	}
	if( topA >= bottomB )
	{
		return false;
	}
	if( rightA <= leftB )
	{
		return false;
	}
	if( leftA >= rightB )
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

void towerTargeting( GameData *game_data, Window *myWindow )
{
	std::list<Tower*>::iterator it;

	//For all the towers on the screen
	for( it=game_data->get_AllTowers()->begin(); it != game_data->get_AllTowers()->end(); it++)
	{
		if(*it != NULL && (*it)->getPlanted() != false)
		{
			//If the tower has no target
			if((*it)->getTarget() == NULL )
			{
				std::list<Enemy*>::iterator it2;
				float closest = 1000000.0;
				Enemy* newTarget = NULL;
				//For all the Enemies on the screen find the closest one
				for( it2=game_data->get_AllEnemies()->begin(); it2 != game_data->get_AllEnemies()->end(); it2++)
				{
					float current = targetDistance( *it, *it2, myWindow );

					if( current < closest)
					{
						closest = current;
						newTarget = (*it2);
					}
				}
				if( closest < (*it)->getRange() )
				{
					(*it)->setTarget( newTarget );
				}
			}
			else
			{
				Enemy* currentTarget = (*it)->getTarget();
				if( targetDistance( *it, currentTarget, myWindow ) > (*it)->getRange() )
				{
					(*it)->setTarget( NULL );
				}
			}
		}
	}
}

bool mainGame( Window *myWindow, GameData *game_data )
{
	//Quit flag
	bool gameQuit = false;

	int enemy_counter = 0;
	int ID_number = 1;
	int wave_size = 0;

	//The frame rate regulator
	Timer *fps = new Timer();
	Timer *delta = new Timer();
	Timer *update = new Timer();

	//When should the next wave spawn?
	Timer *spawnTimer = new Timer();

	//Timer so you dont check for new targets every loop (heavy load)
	Timer *rangeCheck = new Timer();

	//The Resources counter
	Resources* myResources = new Resources();
	
	UI* myUI = new UI( game_data );

	delta->start();
	update->start();
	spawnTimer->start();
	rangeCheck->start();

	//Play the music
	if( Mix_PlayMusic( game_data->get_music(), -1 ) == -1 )
	{
		return 1;
	}

	//While the user hasn't quit
	while( gameQuit == false && myResources->get_lives() > 0 )
	{
		//Start the frame timer
		fps->start();

		//While there's events to handle
		while( SDL_PollEvent( game_data->get_EventStruc() ) )
		{
			//Handle events for the window resizing
			myWindow->handle_events( game_data );

			//Handle camera movement
			myWindow->get_camera()->handle_input( myWindow, game_data );

			//Handle Button presses and clicking on Towers/Enemies
			myUI->handle_events( game_data, myResources, myWindow );

			//If the user wants to quit
			if( ( game_data->get_EventStruc()->type == SDL_KEYDOWN ) && ( game_data->get_EventStruc()->key.keysym.sym == SDLK_ESCAPE ) )
			{
				gameQuit = true;
			}
			
			//If the user has Xed out the window
			if( game_data->get_EventStruc()->type == SDL_QUIT )
			{
				//Quit the program
				gameQuit = true;
			}
		}

		waveSpawner( game_data, wave_size, enemy_counter, ID_number, myResources, spawnTimer, myWindow );

		//Move the camera
		myWindow->get_camera()->move( delta->get_ticks(), myWindow );

		delta->start();

		//Show the background
		SDL_FillRect( myWindow->get_screen(), game_data->get_rect( "background" ), SDL_MapRGB( myWindow->get_screen()->format, 0x00, 0x00, 0x00 ) );
		apply_surface( 0, 0, game_data->get_surface( "background" ), myWindow->get_screen(), myWindow->get_camera() );

		//Shoot if you have a target
		std::list<Tower*>::iterator atk_it;

		for( atk_it=game_data->get_AllTowers()->begin(); atk_it != game_data->get_AllTowers()->end(); atk_it++)
		{
			(*atk_it)->handleAttack( myWindow );
		}

		//Loops through all the towers in AllTowers and calls show()
		applyTowers( myWindow, game_data, myUI );

		//Loops through all the enemies in AllEnemies and calls show()
		applyEnemies( game_data, myResources, delta, &enemy_counter, myUI, myWindow );

		myUI->showTopBar( myWindow, game_data, myResources );
		myUI->showBotUI( myWindow, game_data );

		//targeting
		//Check if anything is in range every .5 sec
		if( rangeCheck->get_ticks() > 500)
		{
			towerTargeting( game_data, myWindow );
		}

		//Update the screen
		if( SDL_Flip( myWindow->get_screen() ) == -1 )
		{
			return false;
		}

		//Cap the frame rate
		if( fps->get_ticks() < 1000 / Window::FPS )
		{
			SDL_Delay( ( 1000 / Window::FPS ) - fps->get_ticks() );
		}
	}

	//Some clean up when done
	game_data->get_AllEnemies()->clear();
	game_data->get_AllTowers()->clear();
	myWindow->get_camera()->resetCamera();

	//exited properly
	return true;
}

//Add to data structure you stupid person
bool menu( Window *myWindow, GameData *game_data )
{
	//Quit flag
	bool quit = false;
	bool showInstructions = false;
	bool showOptions = false;

	Timer *fps = new Timer();
	Timer *delta = new Timer();

	std::stringstream text;

	myButton* playButton = new myButton( game_data->get_rect ("playButton") );
	myButton* instructions = new myButton( game_data->get_rect ("instructionButton") );
	myButton* options = new myButton( game_data->get_rect("optionButton") );

	Enemy* god = new Enemy( 42, 1000000, 200, 1, myWindow, game_data);

	Tower* T1 = new Tower( game_data->get_surface("cannonTower"), 1, myWindow->get_ScreenWidth()/5, (myWindow->get_ScreenHeight()*2)/3, game_data->get_sound("laserSound3"), myWindow, game_data );
	Tower* T2 = new Tower( game_data->get_surface("laserTower"), 5, (myWindow->get_ScreenWidth()*3)/4, (myWindow->get_ScreenHeight()*2)/3 + 200, game_data->get_sound("laserSound3"), myWindow, game_data );
	Tower* T3 = new Tower( game_data->get_surface("nukeTower"), 4, (myWindow->get_ScreenWidth()*4)/5, (myWindow->get_ScreenWidth()*1)/5, game_data->get_sound("laserSound3"), myWindow, game_data );
	T1->setRange( 3000 );
	T2->setRange( 3000 );
	T3->setRange( 3000 );
	T1->setTarget( god );
	T2->setTarget( god );
	T3->setTarget( god );

	delta->start();

	while( quit == false )
	{
		fps->start();
		//While there's events to handle
		while( SDL_PollEvent( game_data->get_EventStruc() ) )
		{
			myWindow->handle_events( game_data );

			//If the user has pressed at key
			if( (game_data->get_EventStruc()->type == SDL_KEYDOWN) && ( game_data->get_EventStruc()->key.keysym.sym == SDLK_ESCAPE ) )
			{
				if( showOptions == false && showInstructions == false )
				{
					quit = true;
				}
				else
				{
					showOptions = false;
					showInstructions = false;
				}
			}


			if( playButton->handle_events( game_data ) == true )
			{
				#ifdef DEBUG
					debugf << "Play button clicked!"  << std::endl;
					debugf.flush();
				#endif
				//Pressed play so start the game
				return true;

			}

			if( instructions->handle_events( game_data ) == true )
			{
				showInstructions = true;
			}

			if( options->handle_events( game_data ) == true )
			{
				showOptions = true;
			}

			//If the user has Xed out the window
			if( game_data->get_EventStruc()->type == SDL_QUIT )
			{
				//Quit the program
				quit = true;
			}
		}
		delta->start();

		//Show the background
		if( showOptions == false && showInstructions == false )
		{
			apply_surface( 0, 0, game_data->get_surface( "menuBackground" ), myWindow->get_screen() );

			double angle = towerAngle( T1, god );
			SDL_Surface *temp = getRotatedImage( int(angle), game_data->get_surfaceArray( "cannonImages" ) );
			T1->setLastImage( temp );
			T1->handleAttack( myWindow );
			T1->show( temp, myWindow );

			angle = towerAngle( T2, god );
			temp = getRotatedImage( int(angle), game_data->get_surfaceArray( "laserImages" ) );
			T2->setLastImage( temp );
			T2->handleAttack( myWindow );
			T2->show( temp, myWindow );

			angle = towerAngle( T3, god );
			temp = getRotatedImage( int(angle), game_data->get_surfaceArray( "nukeImages" ) );
			T3->setLastImage( temp );
			T3->handleAttack( myWindow );
			T3->show( temp, myWindow );

			god->menuMove( delta->get_ticks(), myWindow );
			god->show( myWindow , game_data );

			//Show the play button
			SDL_FillRect( myWindow->get_screen(), game_data->get_rect( "playButton" ), SDL_MapRGB( myWindow->get_screen()->format, 0xFF, 0xA5, 0x00 ) );
			SDL_FillRect( myWindow->get_screen(), game_data->get_rect( "instructionButton" ), SDL_MapRGB( myWindow->get_screen()->format, 0xFF, 0xA5, 0x00 ) );
			SDL_FillRect( myWindow->get_screen(), game_data->get_rect( "optionButton" ), SDL_MapRGB( myWindow->get_screen()->format, 0xFF, 0xA5, 0x00 ) );

			text << "Tower Defense!";
			apply_text_surface( myWindow->get_ScreenWidth()/2 - 350,  myWindow->get_ScreenHeight()/5, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "large_robot_ttf" ), game_data->get_color( "textColor" ) );
			text.str("");

			text << "PLAY";
			apply_text_surface( game_data->get_rect( "playButton" )->x + game_data->get_rect( "playButton" )->w/2 - 100,  game_data->get_rect( "playButton" )->y + 30, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "large_robot_ttf" ), game_data->get_color( "textColor" ) );
			text.str("");


			text << "Instructions";
			apply_text_surface( game_data->get_rect( "instructionButton" )->x + 30,  game_data->get_rect( "instructionButton" )->y + 30, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "large_robot_ttf" ), game_data->get_color( "textColor" ) );
			text.str("");


			text << "Options";
			apply_text_surface( game_data->get_rect( "optionButton" )->x + 30,  game_data->get_rect( "optionButton" )->y + 30, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "large_robot_ttf" ), game_data->get_color( "textColor" ) );
			text.str("");


			text << "X : " << god->getX() << "  Y : " << god->getX();
			apply_text_surface( 30,  30, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "mid_conviction_ttf" ), game_data->get_color( "textColor" ) );
			text.str("");

		}
		else if( showOptions == true )
		{
			SDL_FillRect( myWindow->get_screen(), game_data->get_rect( "mainMenu" ), SDL_MapRGB( myWindow->get_screen()->format, 0x77, 0x77, 0x77 ) );
			text << "Options";
			apply_text_surface( myWindow->get_ScreenWidth()/3,  myWindow->get_ScreenHeight()/4, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "large_robot_ttf" ), game_data->get_color( "textColor" ) );
			text.str("");
		}
		else if( showInstructions == true )
		{
			SDL_FillRect( myWindow->get_screen(), game_data->get_rect( "mainMenu" ), SDL_MapRGB( myWindow->get_screen()->format, 0x77, 0x77, 0x77 ) );

			text << "Instructions";
			apply_text_surface( myWindow->get_ScreenWidth()/3,  myWindow->get_ScreenHeight()/4, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "large_robot_ttf" ), game_data->get_color( "textColor" ) );
			text.str("");

			text << "-Build towers to stop the enemy ships from getting to the other side of the screen";
			apply_text_surface( myWindow->get_ScreenWidth()/10,  myWindow->get_ScreenHeight()/2 + 90, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "robot_ttf" ), game_data->get_color( "textColor" ) );
			text.str("");

			text << "-When you have enough money LEFT click a tower icon to build a tower";
			apply_text_surface( myWindow->get_ScreenWidth()/10,  myWindow->get_ScreenHeight()/2 + 180, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "robot_ttf" ), game_data->get_color( "textColor" ) );
			text.str("");

			text << "-RIGHT click to plant the tower in the location that you desire";
			apply_text_surface( myWindow->get_ScreenWidth()/10,  myWindow->get_ScreenHeight()/2 + 270, myWindow->get_screen(), text.str().c_str(), game_data->get_font( "robot_ttf" ), game_data->get_color( "textColor" ) );
			text.str("");
		}

		if( god->getHealth() < 100 )
		{
			delete god;
			god = new Enemy( 42, 1000000, 200, 1, myWindow, game_data );
		}

		//Update the screen
		if( SDL_Flip( myWindow->get_screen() ) == -1 )
		{
			return false;
		}

		//Cap the frame rate
		if( fps->get_ticks() < 1000 / Window::FPS )
		{
			SDL_Delay( ( 1000 / Window::FPS ) - fps->get_ticks() );
		}
	}

	return false;
}

void clean_up( Window *myWindow )
{
	if( Mix_PlayingMusic() )
	{
		Mix_FadeOutMusic(1500);
		SDL_Delay(1500);
	}

	//Quit SDL_ttf
	TTF_Quit();

	//Quit SDL
	SDL_Quit();

	#ifdef DEBUG
		debugf.close();
	#endif
	
}

int main( int argc, char* args[] )
{
	bool quit = false;
	TTF_Font *loadingFont = NULL;

	//Initialize SDL
	if( init() == false )
	{
		return 1;
	}

	//All the container objects
	GameData *game_data = new GameData();

	//Create myWindow
	Window *myWindow = new Window();

	#ifdef DEBUG	
	//Open the file where you store all the debug data
	::debugf.open( "Data/debug.txt" );
	
	//Check to see if you were actually able to open the file
	if(!debugf.is_open())
	{
		return 1;
	}
	#endif

	#ifdef DEBUG
		debugf << "Screen width:" << myWindow->get_ScreenWidth()  << " Screen height:" << myWindow->get_ScreenHeight() << " Level width:" << myWindow->get_LevelWidth() << " Level height:" << myWindow->get_LevelHeight() << std::endl;
		debugf.flush();
	#endif

	//Initialize SDL_mixer
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
	{
		return false;
	}

	//File Loading starts...
	SDL_Rect FullScreen = {0, 0, myWindow->get_ScreenWidth(), myWindow->get_ScreenHeight()};
	//The color of the loading font
	SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
	//Dirty load
	loadingFont = TTF_OpenFont( (char*)"Data/Fonts/ROBOM___.TTF", 72 );

	SDL_FillRect( myWindow->get_screen(), &FullScreen , SDL_MapRGB( myWindow->get_screen()->format, 0x00, 0x00, 0x00 ) );

	std::stringstream text;
	text << "Loading :D";
	apply_text_surface( myWindow->get_ScreenWidth()/2, myWindow->get_ScreenHeight()/2, myWindow->get_screen(), text.str().c_str(), loadingFont, textColor );

	//**Ignore** For debug purposes
	#ifdef DEBUG
		debugf << "Loading files..."  << std::endl;
		debugf.flush();
	#endif

	//Load the files
	if( game_data->load_files( myWindow ) == false )
	{
		return 0;
	}

	//**Ignore** For debug purposes
	#ifdef DEBUG
		debugf << "Loading files Complete!"  << std::endl;
		debugf.flush();
	#endif

	//Set all the Rect Coords
	game_data->initAllRectCoords( myWindow );

	//**Ignore** For debug purposes
	#ifdef DEBUG
		debugf << "Rect coords Loaded!"  << std::endl;
		debugf.flush();
	#endif

	game_data->initAllSurfaceArrays();

	//**Ignore** For debug purposes
	#ifdef DEBUG
		debugf << "SurfaceArrays Loaded!"  << std::endl;
		debugf.flush();
	#endif

	game_data->initSampleTowers( myWindow );

		//**Ignore** For debug purposes
	#ifdef DEBUG
		debugf << "SampleTowers Loaded!"  << std::endl;
		debugf.flush();
	#endif

	while( quit == false )
	{
		//**Ignore** For debug purposes
		#ifdef DEBUG
			debugf << "*********ENTERING MENU!*********"  << std::endl;
			debugf.flush();
		#endif

		if( menu( myWindow, game_data ) == false )
		{
			break;
		}

		//Stop the music
		Mix_HaltMusic();

		//**Ignore** For debug purposes
		#ifdef DEBUG
			debugf << "*********ENTERING GAME!*********"  << std::endl;
			debugf.flush();
		#endif

		if( mainGame( myWindow, game_data ) == false )
		{
			return 1;
		}
	}

	#ifdef DEBUG
		debugf << "Game Exited Correctly, Doing Clean-Up"  << std::endl;
		debugf.flush();
	#endif

	//Clean up
	clean_up( myWindow );

	return 0;
}

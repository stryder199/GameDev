//=================================
// include guard
#ifndef ProgramRoot_H_
#define ProgramRoot_H_

//=================================
// included dependencies
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include <vector>

//=================================
// Forward Declared Dependencies
class Device;
class GameData;
class Window;
class Timer;
class Environment;


class ProgramRoot{

private:
	Window *myWindow;
	Device *myDevice;
	GameData *game_data;
	Timer *spawnTimer;
	Timer *gameTimer;
	Environment *world;


public:
	ProgramRoot();
	~ProgramRoot();


	int init( Window *myWindow, Device *myDevice, HINSTANCE hInstance, int nCmdShow );
	void Render( Device *myDevice, GameData *game_data );
	void spawnUnit(Device *myDevice, GameData *game_data);
	void cleanUp(GameData *game_data);
	bool handleMouseEvents( MSG *msg, GameData *game_data );
	bool handleKeyEvents( MSG *msg, GameData *game_data );
	void prepScreen( Device *myDevice );
	int go( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow );

};

#endif /* ProgramRoot_H_ */

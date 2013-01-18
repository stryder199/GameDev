#include "ProgramRoot.h"
#include "Window.h"
#include "Device.h"
#include "GameData.h"
#include "Unit.h"
#include "Timer.h"
#include "Environment.h"

ProgramRoot::ProgramRoot()
{
	myWindow = new Window();
	myDevice = new Device();
	game_data = new GameData();
	spawnTimer = new Timer();
	gameTimer = new Timer();
	//world = new Environment( myDevice );
}

int ProgramRoot::init( Window *myWindow, Device *myDevice, HINSTANCE hInstance, int nCmdShow )
{
	if( FAILED(myWindow->init(hInstance,nCmdShow)) )
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
	}

	if( FAILED( myDevice->init( myWindow->gethWnd() ) ) )
    {
        myDevice->CleanupDevice();
        return 0;
    }

	return 1;	
}

//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void ProgramRoot::Render( Device *myDevice, GameData *game_data )
{
	std::vector<Unit*>::iterator it;

	for ( it=game_data->getAllUnits()->begin() ; it != game_data->getAllUnits()->end(); it++ )
	{
		//Poll for new bullets
		(*it)->handleShooting( myDevice );

		//Do position matrix opperations
		(*it)->calculatePos();
	}

	prepScreen(myDevice);

    // Update variables for all cubes
	for ( it=game_data->getAllUnits()->begin() ; it != game_data->getAllUnits()->end(); it++ )
	{
		(*it)->updateVars(myDevice);
		myDevice->g_pImmediateContext->DrawIndexed( 36, 0, 0 );
	}

    // Present our back buffer to our front buffer
    myDevice->g_pSwapChain->Present( 0, 0 );
}

void ProgramRoot::spawnUnit(Device *myDevice, GameData *game_data)
{
	Unit *newUnit = new Unit(myDevice);
	game_data->getAllUnits()->push_back(newUnit);
}

void ProgramRoot::cleanUp(GameData *game_data)
{
	game_data->~GameData();
}

bool ProgramRoot::handleMouseEvents( MSG *msg, GameData *game_data )
{
	std::vector<Unit*>::iterator it;

	for ( it=game_data->getAllUnits()->begin() ; it != game_data->getAllUnits()->end(); it++ )
	{
		(*it)->eventHandle(msg);
	}

	return true;
}

bool ProgramRoot::handleKeyEvents( MSG *msg, GameData *game_data )
{
	std::vector<Unit*>::iterator it;

	for ( it=game_data->getAllUnits()->begin() ; it != game_data->getAllUnits()->end(); it++ )
	{
		(*it)->eventHandle(msg);
	}

	return true;
}

void ProgramRoot::prepScreen( Device *myDevice )
{

    // Clear the back buffer
    float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red, green, blue, alpha
    myDevice->g_pImmediateContext->ClearRenderTargetView( myDevice->g_pRenderTargetView, ClearColor );


    // Clear the depth buffer to 1.0 (max depth)
    myDevice->g_pImmediateContext->ClearDepthStencilView( myDevice->g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

    //Keep
	myDevice->g_pImmediateContext->VSSetShader( myDevice->g_pVertexShader, NULL, 0 );
	myDevice->g_pImmediateContext->VSSetConstantBuffers( 0, 1, myDevice->g_ppConstantBufferArray );
	myDevice->g_pImmediateContext->PSSetShader( myDevice->g_pPixelShader, NULL, 0 );
}

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int ProgramRoot::go( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	if( init(myWindow,myDevice,hInstance,nCmdShow) == 0)
	{
		return 0;
	}
	
	// Main message loop
    MSG msg = {0};

	spawnTimer->start();
	gameTimer->start();
	spawnUnit( myDevice,  game_data );

    while( WM_QUIT != msg.message )
    {
		/*if(spawnTimer->get_ticks() > 2000)
		{
			spawnUnit( game_data );
			spawnTimer->restart();
		}*/

        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE | PM_NOYIELD ) )
        {
            bool fHandled = false;
			if (msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)
				fHandled = handleMouseEvents( &msg, game_data );
			else if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST)
				fHandled = handleKeyEvents( &msg, game_data );
			else if (WM_QUIT == msg.message)
				break;
		
			if ( !fHandled )
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
        else
        {
			Render( myDevice, game_data );
        }
    }

	cleanUp( game_data );
    myDevice->CleanupDevice();

    return ( int )msg.wParam;
}
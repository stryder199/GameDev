#include "Unit.h"
#include "Bullet.h"
#include "Device.h"
#include "Timer.h"
#include "Model.h"

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

Unit::Unit( Device *myDevice )
{
	//Unit Matrix Location in 3d space
	g_World = new XMMATRIX();
	ZeroMemory(g_World, sizeof(XMMATRIX));
	*g_World = XMMatrixIdentity();

	mesh = new Model();
	mesh->init("Data\cubeConv.txt", myDevice);

	bulletContainer = new std::vector <Bullet *>;
	
	shooting = false;

	unitTimer = new Timer();
	unitTimer->start();

	shootingIntervalTimer = new Timer();
	shootingIntervalTimer->start();

	xVel = 0.0f;
	yVel = 0.0f;
	zVel = 0.0f;
}

XMMATRIX* Unit::getMatrix()
{
	return g_World;
}


//TODO Change pointer to pointer? or data set to data set??(Current imp)
void Unit::setMatrix( XMMATRIX *var )
{
	*g_World = *var;
}

void Unit::updateVars( Device *myDevice )
{
	std::vector<Bullet *>::iterator it;

	// Update variables for all bullets
	for ( it=bulletContainer->begin() ; it != bulletContainer->end(); it++ )
	{
		(*it)->updateVars(myDevice);
		myDevice->g_pImmediateContext->DrawIndexed( 36, 0, 0 );
	}


	ConstantBuffer cb;

	XMMATRIX currentUnitMatrix = *g_World;
	cb.mWorld = XMMatrixTranspose( currentUnitMatrix );

	XMMATRIX viewMatrix = myDevice->g_View;
	cb.mView = XMMatrixTranspose( viewMatrix );

	XMMATRIX projectionMatrix = myDevice->g_Projection;
	cb.mProjection = XMMatrixTranspose( projectionMatrix );

	myDevice->g_pImmediateContext->IASetVertexBuffers( 0, 1, &(mesh->g_pVertexBuffer), &(mesh->stride), &(mesh->offset) );

	myDevice->g_pImmediateContext->IASetIndexBuffer( mesh->g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

	myDevice->g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

void Unit::calculatePos()
{
	//Show all bullets as well
	std::vector<Bullet *>::iterator it;

	for ( it=bulletContainer->begin() ; it != bulletContainer->end(); it++ )
	{
		(*it)->calculatePos();
	}

	// Move Cube Away from screen
	XMMATRIX mTranslate = XMMatrixTranslation( xVel, yVel, zVel );
	XMMATRIX mCurrent = *g_World;
	XMMATRIX mTotal = XMMatrixMultiply(mTranslate, mCurrent);

	*g_World = mTotal;
}

void Unit::eventHandle( MSG *msg )
{
	if ( msg->message == WM_KEYDOWN )
	{
		switch(msg->wParam)
		{
		case VK_UP:
			zVel = 0.001f;
			break;
		case VK_DOWN:
			zVel = -0.001f;
			break;
		case VK_RIGHT:
			xVel = 0.001f;
			break;
		case VK_LEFT:
			xVel = -0.001f;
			break;
		case VK_SPACE:
			shooting = true;
			break;
		}
	}
	else if( msg->message == WM_KEYUP )
	{
		switch(msg->wParam)
		{
		case VK_UP:
			zVel = 0.0f;
			break;
		case VK_DOWN:
			zVel = 0.0f;
			break;
		case VK_RIGHT:
			xVel = 0.0f;
			break;
		case VK_LEFT:
			xVel = 0.0f;
			break;
		case VK_SPACE:
			shooting = false;
			break;
		}
	}
}

void Unit::handleShooting( Device *myDevice )
{
	if(shooting)
	{
		if(shootingIntervalTimer->get_ticks() > 500)
		{
			spawnBullet( myDevice );
			shootingIntervalTimer->restart();
		}
	}
}

void Unit::spawnBullet( Device *myDevice )
{
    Bullet *newBullet = new Bullet(myDevice, this);
	bulletContainer->push_back(newBullet);
}
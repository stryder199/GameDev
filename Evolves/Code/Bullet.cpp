#include "Bullet.h"
#include "Unit.h"
#include "Device.h"
#include "Timer.h"
#include "Model.h"

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

Bullet::Bullet( Device *myDevice, Unit *ownerInit )
{
	owner = ownerInit;

	bulletTimer = new Timer();
	bulletTimer->start();

	mesh = new Model();
	mesh->init("Data\circleConv.txt", myDevice);

	//Unit Matrix Location in 3d space
	g_World = new XMMATRIX();
	ZeroMemory(g_World, sizeof(XMMATRIX));

	XMMATRIX ownerPosition = *ownerInit->getMatrix();
	*g_World = ownerPosition;

	xVel = 0.0f;
	yVel = 0.0f;
	zVel = 0.01f;
}

XMMATRIX* Bullet::getMatrix()
{
	return g_World;
}

//TODO Change pointer to pointer? or data set to data set??(Current imp)
void Bullet::setMatrix( XMMATRIX *var )
{
	*g_World = *var;
}

void Bullet::updateVars( Device *myDevice )
{
	/*ConstantBuffer cb;

	XMMATRIX currentUnitMatrix = *g_World;
	cb.mWorld = XMMatrixTranspose( currentUnitMatrix );

	XMMATRIX viewMatrix = myDevice->View;
	cb.mView = XMMatrixTranspose( viewMatrix );

	XMMATRIX projectionMatrix = myDevice->Projection;
	cb.mProjection = XMMatrixTranspose( projectionMatrix );

	myDevice->pDevContext->UpdateSubresource( myDevice->pConstantBuffer, 0, NULL, &cb, 0, 0 );*/

	myDevice->g_pImmediateContext->UpdateSubresource( mesh->g_pConstantBuffer, 0, NULL, &cb, 0, 0 );

}

void Bullet::calculatePos()
{
	// Move Cube Away from screen
	XMMATRIX mTranslate = XMMatrixTranslation( xVel, yVel, zVel );
	//XMMATRIX mScale = XMMatrixScaling( 0.3f, 0.3f, 0.3f );
	XMMATRIX mSubTotal =  mTranslate;

	XMMATRIX mCurrent = *g_World;
	XMMATRIX mTotal = XMMatrixMultiply(mSubTotal, mCurrent);

	*g_World = mTotal;
}
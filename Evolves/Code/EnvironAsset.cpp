#include "EnvironAsset.h"
#include "Model.h"

EnvironAsset::EnvironAsset( Device *myDevice )
{
	mesh = new Model();
	mesh->setSquare(myDevice, XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ));
}


EnvironAsset::~EnvironAsset()
{

}

HRESULT EnvironAsset::Draw()
{
	return S_OK;
}
#pragma once

//////////////
// INCLUDES //
#include <d3d11.h>
#include <DirectXMath.h>

///////////////////////
// MY CLASS INCLUDES //
#include "ModelClass.h"

/////////////////
// FORWARD DEC //

using namespace DirectX;

class BitmapClass : public ModelClass
{
public:
	BitmapClass();
	BitmapClass(const BitmapClass&);
	~BitmapClass();

	bool Initialize(MeshClass* mesh, LightClass *lightSource, XMFLOAT2 pos, XMFLOAT2 scale);
	void Shutdown();
	bool Render(ShaderControllerClass* shader);
private:
	bool PreProcessing();
};

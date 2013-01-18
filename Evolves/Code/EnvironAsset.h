//=================================
// include guard
#ifndef EnvironAsset_H_
#define EnvironAsset_H_

//=================================
// included dependencies
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

//=================================
// forward declared dependencies
class Model;
class Device;

class EnvironAsset{

private:
	Model *mesh;

public:
	EnvironAsset( Device *myDevice );
	~EnvironAsset();

	HRESULT Draw();
};

#endif /* EnvironAsset_H_ */
//=================================
// include guard
#ifndef Environment_H_
#define Environment_H_

//=================================
// included dependencies
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include <vector>;

//=================================
// forward declared dependencies
class Device;
class EnvironAsset;

class Environment{

private:
	std::vector<EnvironAsset*>* stars; 

public:
	Environment( Device *myDevice );
	~Environment();

	HRESULT Draw();
};

#endif /* Environement_H_ */
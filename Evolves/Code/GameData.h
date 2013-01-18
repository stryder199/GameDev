//=================================
// include guard
#ifndef GameData_H_
#define GameData_H_

//=================================
// forward declared dependencies
class Unit;

//=================================
// included dependencies
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include <vector>;

class GameData
{
private:
	std::vector<Unit*>* allUnits;

public:
	GameData();
	~GameData();

	std::vector<Unit*>* getAllUnits();
	int loadFiles();
};

#endif /* GameData_H_ */
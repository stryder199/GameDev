#pragma once

//////////////
// INCLUDES //
#include <DirectXMath.h>
#include <vector>
#include <D3D11.h>
#include <mutex>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class ShaderControllerClass;
class ModelClass;

using namespace std;

class ThreeDGraphicsClass{
public:
	ThreeDGraphicsClass();
	~ThreeDGraphicsClass();

	bool Initialize();

	bool RenderAll(ShaderControllerClass* shader);

	bool AddPlayer(string meshname, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, int totalHealth, int totalShields, int totalEnergy, int energyCost, int torpedos);
	bool AddStar(string meshname, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotVel);
	bool AddPlanet(string meshname, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotVel);
private:
	mutex modelMutex;
	vector<ModelClass*> m_allModels;
};
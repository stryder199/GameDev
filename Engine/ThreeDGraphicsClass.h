#pragma once

//////////////
// INCLUDES //
#include <DirectXMath.h>
#include <vector>
#include <mutex>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class ShaderControllerClass;
class ModelClass;
class EventClass;
class ShipClass;

using namespace std;
using namespace DirectX;

class ThreeDGraphicsClass{
public:
    ThreeDGraphicsClass();
    ~ThreeDGraphicsClass();

    void Initialize();
    void Shutdown();

    void PreProcessing();
    void RenderAll(ShaderControllerClass* shader);
    
    void ConstructFrustum();
    bool CheckSphereAgainstFrustum(XMFLOAT3 pos, float radius);

    void AddPlayer(string meshname, XMFLOAT3 pos, XMFLOAT3 scale, int totalHealth, int totalShields, int totalEnergy, int energyCost, int torpedos);
    void AddShip(string meshname, XMFLOAT3 pos, XMFLOAT3 scale, int totalHealth, int totalShields, int totalEnergy, int energyCost, int torpedos);
    void AddStar(string meshname, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotVel);
    void AddPlanet(string meshname, XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 rotVel);
    void HandleEvents(EventClass* events);
private:
    mutex modelMutex;
    vector<ModelClass*> m_allModels;
    XMFLOAT4 m_fulstrumPlanes[6];
    ShipClass* m_player;
};
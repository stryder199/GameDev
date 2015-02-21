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

class ThreeDGraphicsClass{
public:
    ThreeDGraphicsClass();
    ~ThreeDGraphicsClass();

    void Initialize();
    void Shutdown();

    void PreProcessing();
    void RenderAll(ShaderControllerClass* shader);
    
    void ConstructFrustum();
    bool CheckSphereAgainstFrustum(DirectX::XMFLOAT3 pos, float radius);

    void AddPlayer(std::string meshname, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, int totalHealth, int totalShields, int totalEnergy, int energyCost, int torpedos);
    void AddShip(std::string meshname, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, int totalHealth, int totalShields, int totalEnergy, int energyCost, int torpedos);
    void AddStar(std::string meshname, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotVel);
    void AddPlanet(std::string meshname, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 rotVel);
    void HandleEvents(EventClass* events);
private:
    std::mutex modelMutex;
    std::vector<ModelClass*> m_allModels;
    DirectX::XMFLOAT4 m_fulstrumPlanes[6];
    ShipClass* m_player;
};
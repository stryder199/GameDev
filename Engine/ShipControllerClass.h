#pragma once

//////////////
// INCLUDES //
#include <DirectXMath.h>
#include <memory>

///////////////////////
// MY CLASS INCLUDES //
#include <mutex>
#include <vector>

/////////////////
// FORWARD DEC //
class ShipClass;

class ShipControllerClass
{
public:
    ~ShipControllerClass();
    
    static ShipControllerClass* GetInstance();

    void AddShip(std::shared_ptr<ShipClass> ship);
    void AddPlayer(std::shared_ptr<ShipClass> ship);

    std::shared_ptr<ShipClass> GetPlayer();

    void PreProcessing();

private:
    ShipControllerClass();
    ShipControllerClass(const ShipControllerClass&) {};
    ShipControllerClass& operator=(const ShipControllerClass&) {};

    void TargetEnemyShips();
    void CheckBulletCollisions();
    void RemoveDeadShips();

    static ShipControllerClass* m_pInstance;
    static std::mutex instanceMutex;

    std::vector<std::shared_ptr<ShipClass>> m_allShips;
    std::shared_ptr<ShipClass> m_player;
};


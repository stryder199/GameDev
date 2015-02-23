#include "ShipControllerClass.h"
#include "ShipClass.h"
#include "BulletClass.h"
#include "VectorHelpers.h"

using namespace std;

ShipControllerClass* ShipControllerClass::m_pInstance = nullptr;
mutex ShipControllerClass::instanceMutex;

ShipControllerClass::ShipControllerClass()
{
    m_allShips = vector<shared_ptr<ShipClass>>();
    m_player = nullptr;
}

ShipControllerClass::~ShipControllerClass()
{
}

ShipControllerClass* ShipControllerClass::GetInstance()
{
    instanceMutex.lock();
    if (!m_pInstance)
        m_pInstance = new ShipControllerClass();
    instanceMutex.unlock();

    return m_pInstance;
}

void ShipControllerClass::AddShip(shared_ptr<ShipClass> ship)
{
    m_allShips.push_back(ship);
}

void ShipControllerClass::AddPlayer(shared_ptr<ShipClass> ship)
{
    m_allShips.push_back(ship);
    m_player = ship;
}

void ShipControllerClass::PreProcessing()
{
    TargetEnemyShips();
    CheckBulletCollisions();
    RemoveDeadShips();
}

void ShipControllerClass::TargetEnemyShips()
{
    // Make sure each ship has a target
    for (shared_ptr<ShipClass> &ship : m_allShips)
    {
        // If the ship doesn't have a target
        if (ship->GetTargetShip() == nullptr && ship != m_player)
        {
            float currentDistance = 0.0;
            // Search for the closest ship
            for (shared_ptr<ShipClass> &targetShip : m_allShips)
            {
                // The ship cant target itself
                if (targetShip != ship)
                {
                    float newDistance = VectorHelpers::Distance(ship->getPosition(), targetShip->getPosition());
                    // If we don't have a ship already
                    // Or see if this ship is closer than the currentTarget
                    if (ship->GetTargetShip() == nullptr || newDistance > currentDistance)
                    {
                        ship->SetNewTarget(targetShip);
                        currentDistance = newDistance;
                    }
                }
            }
        }
    }
}

void ShipControllerClass::CheckBulletCollisions()
{
    // Check collisions against each ship
    for (shared_ptr<ShipClass> &ship : m_allShips)
    {
        // Check the bullets origininating from each ship
        for (shared_ptr<ShipClass> &otherShips : m_allShips)
        {
            // Cant shoot yourself (stops bug where as bullet exits the barrel, the bullet doesnt collide with the shooter)
            if (ship != otherShips)
            {
                for (BulletClass* &bullet : *(otherShips->GetBullets()))
                {
                    if (VectorHelpers::SphereSphereCollision(ship->getPosition(), ship->getScale().x, bullet->getPosition(), bullet->getScale().x))
                    {
                        ship->Hit(bullet->GetDamage());
                    }
                }
            }
        }
    }
}

void ShipControllerClass::RemoveDeadShips()
{
    // Check collisions against each ship
    for (vector<shared_ptr<ShipClass>>::iterator shipIt = m_allShips.begin(); shipIt != m_allShips.end();)
    {
        if ((*shipIt)->IsDead())
        {
            shipIt = m_allShips.erase(shipIt);
        }
        else
            ++shipIt;
    }
}

shared_ptr<ShipClass> ShipControllerClass::GetPlayer()
{
    return m_player;
}
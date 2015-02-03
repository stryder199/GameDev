#include "ISingleton.h"

using namespace std;

ISingleton* ISingleton::m_pInstance = nullptr;
mutex ISingleton::instanceMutex;


ISingleton* ISingleton::getInstance()
{
    instanceMutex.lock();
    if (!m_pInstance)
        m_pInstance = new ISingleton();
    instanceMutex.unlock();

    return m_pInstance;
}


ISingleton::ISingleton()
{
}


ISingleton::~ISingleton()
{
}

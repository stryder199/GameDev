#pragma once
#include <mutex>

class ISingleton
{
public:
    ~ISingleton();
    static ISingleton* getInstance();
protected:
    ISingleton();
private:
    ISingleton(const ISingleton&);
    ISingleton& operator=(const ISingleton&) {};

    static ISingleton* m_pInstance;
    static std::mutex instanceMutex;

};


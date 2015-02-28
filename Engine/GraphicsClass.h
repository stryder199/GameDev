#pragma once

//////////////
// INCLUDES //
#include <future>
#include <map>
#include <DirectXMath.h>
#include <rapidxml\rapidxml.hpp>

///////////////////////
// MY CLASS INCLUDES //
#include "Timer.h"

/////////////////
// FORWARD DEC //
class ShaderControllerClass;
class TwoDGraphicsClass;
class ThreeDGraphicsClass;
class EventClass;

struct ShipData
{
public:
    ShipData()
    {
    }
    std::string ShipName;
    std::string MeshName;
    DirectX::XMFLOAT3 Scale;
    int TotalHealth;
    int TotalShields;
    int TotalEnergy;
    int EnergyCost;
    int NumberOfTorpedos;
};

class GraphicsClass{

public:
    GraphicsClass();
    ~GraphicsClass();

    void Initialize(int);
    void Shutdown();

    void PreProcessing();
    void Render();

    void LoadData();
    void HandleEvents(EventClass* events);

private:
    void LoadGameData(std::string filename);
    void LoadMeshData(rapidxml::xml_node<> *meshNode);
    std::map<std::string, ShipData>* LoadShipData(rapidxml::xml_node<> *shipNode);
    void LoadObjectData(rapidxml::xml_node<> *modelNode, std::map<std::string, ShipData>* shipData);
    void LoadBackgroundStars(int starCount);
    void LoadFontData(rapidxml::xml_node<> *meshNode);

private:
    ShaderControllerClass* m_Shader;
    TwoDGraphicsClass* m_2DGraphics;
    ThreeDGraphicsClass* m_3DGraphics;
    Timer m_fpsTimer;
    int m_fps, m_fpsCount;
};
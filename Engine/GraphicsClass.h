#pragma once

//////////////
// INCLUDES //
#include <future>

///////////////////////
// MY CLASS INCLUDES //
#include "Timer.h"

#define Async

/////////////////
// FORWARD DEC //
class ShaderControllerClass;
class TwoDGraphicsClass;
class ThreeDGraphicsClass;
class EventClass;

using namespace std;

class GraphicsClass{

public:
    GraphicsClass();
    ~GraphicsClass();

    void Initialize(int);
    void Shutdown();

    void PreProcessing();
    void Render();

    void LoadGameData();
    void LoadMeshData(std::string filename);
    void LoadObjectData(std::string filename);
    void GenerateStars(int starCount);
    void HandleEvents(EventClass* events);
private:
    ShaderControllerClass* m_Shader;
    TwoDGraphicsClass* m_2DGraphics;
    ThreeDGraphicsClass* m_3DGraphics;
    Timer m_fpsTimer;
    int m_fps, m_fpsCount;
};
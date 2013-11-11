#pragma once

//////////////
// INCLUDES //
#include <Windows.h>
#include <future>

///////////////////////
// MY CLASS INCLUDES //
#include "Timer.h"

/////////////////
// FORWARD DEC //
class ShaderControllerClass;
class TwoDGraphicsClass;
class ThreeDGraphicsClass;

using namespace std;

class GraphicsClass{

public:
	GraphicsClass();
	~GraphicsClass();

	bool Initialize(HINSTANCE, int);
	bool Render();

	bool LoadGameData();
	bool LoadMeshData(string filename);
	bool LoadObjectData(string filename);
	bool GenerateStars(int starCount);
private:
	ShaderControllerClass* m_Shader;
	TwoDGraphicsClass* m_2DGraphics;
	ThreeDGraphicsClass* m_3DGraphics;
	Timer m_fpsTimer;
	int m_fps, m_fpsCount;
};
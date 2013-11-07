#pragma once

//////////////
// INCLUDES //
#include <d3d11.h>
#include <vector>
#include <map>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class BitmapClass;
class TextClass;
class LightClass;
class ShaderControllerClass;

using namespace std;

class TwoDGraphicsClass{

public:
	TwoDGraphicsClass();
	TwoDGraphicsClass(const TwoDGraphicsClass&);
	~TwoDGraphicsClass();

	bool Initialize();

	bool RenderAll(ShaderControllerClass* shader, int fps);

private:
	vector<BitmapClass*> m_allBitmaps;
	map<string, TextClass*> m_allText;
	LightClass* m_DefaultLightSource;
};
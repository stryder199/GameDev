//=================================
// include guard
#pragma once

//=================================
// forward declared dependencies
class BitmapClass;
class TextClass;
class ShaderControllerClass;
class LightClass;

//=================================
// included dependencies
#include <d3d11.h>
#include <vector>

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
	vector<TextClass*> m_allText;
	LightClass* m_DefaultLightSource;
};
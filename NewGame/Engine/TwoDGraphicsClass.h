#pragma once

//////////////
// INCLUDES //
#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>
#include <map>
#include <mutex>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //
class BitmapClass;
class TextClass;
class LightClass;
class ShaderControllerClass;
class FontClass;

using namespace std;

class TwoDGraphicsClass{
public:
	TwoDGraphicsClass();
	~TwoDGraphicsClass();

	bool Initialize();

	bool RenderAll(ShaderControllerClass* shader, int fps);

	bool AddFont(string name, string fontFilename, string textureFilename);
	bool AddText(string name, string initText, string fontname, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT4 color);
	bool AddBitmap(string meshname, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale);
private:
	mutex fontMutex, bitmapMutex, textMutex;
	map<string, FontClass*> m_allFont;
	vector<BitmapClass*> m_allBitmaps;
	map<string, TextClass*> m_allText;
	LightClass* m_DefaultLightSource;
};
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
using namespace DirectX;

class TwoDGraphicsClass{
public:
    TwoDGraphicsClass();
    ~TwoDGraphicsClass();

    void Initialize();
    void Shutdown();

    void RenderAll(ShaderControllerClass* shader, int fps);

    void AddFont(string name, string fontFilename, string textureFilename);
    void AddText(string name, string initText, string fontname, XMFLOAT2 pos, XMFLOAT2 scale, XMFLOAT4 color);
    void AddBitmap(string meshname, XMFLOAT2 pos, XMFLOAT2 scale);
private:
    mutex fontMutex, bitmapMutex, textMutex;
    map<string, FontClass*> m_allFont;
    vector<BitmapClass*> m_allBitmaps;
    map<string, TextClass*> m_allText;
    LightClass* m_DefaultLightSource;
};
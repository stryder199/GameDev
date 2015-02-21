#pragma once

//////////////
// INCLUDES //
#include <DirectXMath.h>
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

class TwoDGraphicsClass{
public:
    TwoDGraphicsClass();
    ~TwoDGraphicsClass();

    void Initialize();
    void Shutdown();

    void RenderAll(ShaderControllerClass* shader, int fps);

    void AddFont(std::string name, std::string fontFilename, std::string textureFilename);
    void AddText(std::string name, std::string initText, std::string fontname, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT4 color);
    void AddBitmap(std::string meshname, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale);
private:
    std::mutex fontMutex, bitmapMutex, textMutex;
    std::map<std::string, FontClass*> m_allFont;
    std::vector<BitmapClass*> m_allBitmaps;
    std::map<std::string, TextClass*> m_allText;
    LightClass* m_DefaultLightSource;
};
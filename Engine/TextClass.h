#pragma once

//////////////
// INCLUDES //
#include <string>
#include <D3D11.h>
#include <DirectXMath.h>

///////////////////////
// MY CLASS INCLUDES //
#include "ModelClass.h"

/////////////////
// FORWARD DEC //
class FontClass;

using namespace std;
using namespace DirectX;

class TextClass : public ModelClass
{
public:
    TextClass();
    ~TextClass();

    void Initialize(string initText, FontClass* font, XMFLOAT2 pos, XMFLOAT2 scale, XMFLOAT4 color);
    void Shutdown();

    void UpdateText(string newText);
    void Render(ShaderControllerClass* shader);
    virtual void PreProcessing();

private:
    void BuildTextMesh(const char* sentence);

    FontClass* m_font;
    string m_text;
    XMFLOAT2 m_screenPos;
};


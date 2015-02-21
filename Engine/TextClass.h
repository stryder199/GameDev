#pragma once

//////////////
// INCLUDES //
#include <string>
#include <DirectXMath.h>

///////////////////////
// MY CLASS INCLUDES //
#include "ModelClass.h"

/////////////////
// FORWARD DEC //
class FontClass;

class TextClass : public ModelClass
{
public:
    TextClass();
    ~TextClass();

    void Initialize(std::string initText, FontClass* font, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT4 color);
    void Shutdown();

    void UpdateText(std::string newText);
    void Render(ShaderControllerClass* shader);
    virtual void PreProcessing();

private:
    void BuildTextMesh(const char* sentence);

    FontClass* m_font;
    std::string m_text;
    DirectX::XMFLOAT2 m_screenPos;
};


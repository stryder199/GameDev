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

using namespace DirectX;

class TextClass : public ModelClass
{
public:
	TextClass();
	~TextClass();

	bool Initialize(std::string initText, FontClass* font, XMFLOAT2 pos, XMFLOAT2 scale, XMFLOAT4 color);
	void Shutdown();

	bool UpdateText(std::string newText);
	bool Render(ShaderControllerClass* shader);
private:
	bool PreProcessing();
	bool BuildTextMesh(const char* sentence);

	FontClass* m_font;
	std::string m_text;
	XMFLOAT2 m_screenPos;
};


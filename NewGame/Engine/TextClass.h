#pragma once

#include <string>
#include <D3D11.h>
#include <DirectXMath.h>
#include "ModelClass.h"

class ShaderControllerClass;
class LightClass;
class FontClass;

using namespace DirectX;

class TextClass : public ModelClass
{
public:
	TextClass();
	~TextClass();

	bool Initialize(std::string initText, FontClass* font, XMFLOAT2 pos, XMFLOAT4 color);
	void Shutdown();

	bool Render(ShaderControllerClass* shader);
private:
	bool PreProcessing();
	bool BuildTextMesh(const char* sentence);

	FontClass* m_font;
	std::string m_text;
};


#pragma once

//////////////
// INCLUDES //
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>

///////////////////////
// MY CLASS INCLUDES //
#include "textureclass.h"

/////////////////
// FORWARD DEC //

using namespace std;

class FontClass
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(string fontPropFilename, string textureFilename);
	void Shutdown();

	TextureClass* GetTexture();
	FontClass::FontType* GetFont();
private:
	bool LoadFontData(string filename);
	void ReleaseFontData();

private:
	FontType* m_Font;
	TextureClass* m_Texture;
};

#pragma once

//////////////////////
///Includes
#include <string>

//////////////////////
///Forward Dec
class BumpMapClass;

using namespace std;

class TextureClass
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	TextureClass(void);
	~TextureClass(void);

	bool Initialize(string textureFilename, string bumpmapFilename, unsigned int textureUnit, bool textureWrap, unsigned int bumpmapUnit, bool bumpmapWrap);
	void Shutdown();

	unsigned int getTextureUnit();
	bool getWrap();

	BumpMapClass* getBumpmap();

private:

	bool loadTextureFromFile(string filename, unsigned int textureUnit, bool wrap);

	unsigned int mTextureID, mTextureUnit;
	bool loaded, mWrap;

	BumpMapClass* mBumpMap;
};

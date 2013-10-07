#pragma once

//////////////////////
///Includes
#include <iostream>

//////////////////////
///Forward Dec

using namespace std;

class BumpMapClass
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
	BumpMapClass(void);
	~BumpMapClass(void);

	bool Initialize(string filename, unsigned int textureUnit, bool wrap);
	void Shutdown();

	unsigned int getTextureUnit();
	bool getWrap();
private:
	bool LoadTextureFromFile(string filename, unsigned int textureUnit, bool wrap);

	bool loaded, mWrap;
	unsigned int mTextureID, mTextureUnit;

};

///////////////////////////////////////////////////////////////////////////////
// Filename: fontclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "fontclass.h"
#include "WindowsHelpers.h"

FontClass::FontClass()
{
	m_Font = 0;
	m_Texture = 0;
}


FontClass::FontClass(const FontClass& other)
{
}


FontClass::~FontClass()
{
}

bool FontClass::Initialize(string fontFilename, string textureFilename)
{
	bool result;


	// Load in the text file containing the font data.
	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	m_Texture = new TextureClass();
	// Load the texture that has the font characters on it.
	result = m_Texture->Initialize(toWChar(textureFilename));
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	// Release the font texture.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	// Release the font data.
	ReleaseFontData();

	return;
}

bool FontClass::LoadFontData(string filename)
{
	ifstream fin;
	int i;
	char temp;


	// Create the font spacing buffer.
	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	// Read in the font size and spacing between chars.
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for (i = 0; i<95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}


void FontClass::ReleaseFontData()
{
	// Release the font data array.
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = 0;
	}

	return;
}

TextureClass* FontClass::GetTexture()
{
	return m_Texture;
}

FontClass::FontType* FontClass::GetFont()
{
	return m_Font;
}
#include "fontclass.h"
#include "WindowsHelpers.h"
#include "GenericException.h"
#include "TextureClass.h"
#include <fstream>

using namespace std;

FontClass::FontClass()
{
    m_Font = nullptr;
    m_Texture = nullptr;
}

FontClass::~FontClass()
{
}

void FontClass::Initialize(string fontFilename, string textureFilename)
{
    // Load in the text file containing the font data.
    LoadFontData(fontFilename);

    m_Texture = new TextureClass();
    // Load the texture that has the font characters on it.
    m_Texture->Initialize(WindowsHelpers::ToLWideStr(textureFilename));
}

void FontClass::LoadFontData(string filename)
{
    ifstream fin;
    int i;
    char temp;

    // Create the font spacing buffer.
    m_Font = new FontType[95];
    // Read in the font size and spacing between chars.
    fin.open(filename);
    if (fin.fail())
    {
        throw GenericException("File not found");
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
}


void FontClass::ReleaseFontData()
{
    // Release the font data array.
    if (m_Font)
    {
        delete[] m_Font;
        m_Font = 0;
    }
}

TextureClass* FontClass::GetTexture()
{
    return m_Texture;
}

FontClass::FontType* FontClass::GetFont()
{
    return m_Font;
}
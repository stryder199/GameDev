#pragma once

//////////////
// INCLUDES //
#include <string>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //]
class TextureClass;

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
    ~FontClass();

    void Initialize(std::string fontPropFilename, std::string textureFilename);

    TextureClass* GetTexture();
    FontType* GetFont();
private:
    void LoadFontData(std::string filename);
    void ReleaseFontData();

private:
    FontType* m_Font;
    TextureClass* m_Texture;
};

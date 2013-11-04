#include "TwoDGraphicsClass.h"
#include "MeshClass.h"
#include "TextClass.h"
#include "FontClass.h"
#include "BitmapClass.h"
#include "D3DClass.h"
#include "ShaderControllerClass.h"
#include "CameraClass.h"
#include "LightClass.h"

TwoDGraphicsClass::TwoDGraphicsClass()
{
	m_DefaultLightSource = 0;
}

TwoDGraphicsClass::TwoDGraphicsClass(const TwoDGraphicsClass& other)
{
}

TwoDGraphicsClass::~TwoDGraphicsClass()
{
}

bool TwoDGraphicsClass::Initialize()
{
	bool result;

	m_DefaultLightSource = new LightClass();

	m_DefaultLightSource->SetAmbientColor(1.0, 1.0, 1.0, 1.0);
	m_DefaultLightSource->SetDiffuseColor(1.0, 1.0, 1.0, 1.0);
	m_DefaultLightSource->SetDirection(0.0, 0.0, -1.0);

	MeshClass *seafloor = new MeshClass();
	result = seafloor->Initialize("data/2dtex.3dmodel", MeshClass::TWOD);
	if (!result)
		return false;

	FontClass *font = new FontClass();
	result = font->Initialize("data/fontdata.txt", L"data/font.dds");
	if (!result)
		return false;

	TextClass *text = new TextClass();
	result = text->Initialize("Test Test Test.", font, XMFLOAT2(0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	if (!result)
		return false;
	m_allText.push_back(text);

	BitmapClass *bitmap = new BitmapClass();
	result = bitmap->Initialize(seafloor, m_DefaultLightSource, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));
	if(!result)
		return false;
	//m_allBitmaps.push_back(bitmap);

	return true;
}

bool TwoDGraphicsClass::RenderAll(ShaderControllerClass* shader)
{
	bool result;

	shader->Set2DShaders();

	//Turn off the ZBuffer since 2D elements have no Z component
	D3DClass::getInstance()->TurnZBufferOff();

	vector<BitmapClass*>::iterator bitmap;
	for (bitmap = m_allBitmaps.begin(); bitmap != m_allBitmaps.end(); ++bitmap)
	{
		result = (*bitmap)->Render(shader);
		if (!result)
			return false;
	}

	shader->SetTextShaders();

	D3DClass::getInstance()->TurnOnAlphaBlending();
	vector<TextClass*>::iterator text;
	for (text = m_allText.begin(); text != m_allText.end(); ++text)
	{
		result = (*text)->Render(shader);
		if (!result)
			return false;
	}
	D3DClass::getInstance()->TurnOffAlphaBlending();

	//Turn the ZBuffer back on for future 3d rendering
	D3DClass::getInstance()->TurnZBufferOn();

	return true;
}
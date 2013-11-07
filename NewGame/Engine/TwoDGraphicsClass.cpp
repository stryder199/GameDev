#include "TwoDGraphicsClass.h"
#include "MeshClass.h"
#include "TextClass.h"
#include "FontClass.h"
#include "BitmapClass.h"
#include "D3DClass.h"
#include "ShaderControllerClass.h"
#include "CameraClass.h"
#include "LightClass.h"
#include "WindowClass.h"
#include "PlayerClass.h"

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

	float top = WindowClass::getInstance()->getScreenHeight() / 2.0f;
	float bottom = -1 * top;
	float right = WindowClass::getInstance()->getScreenWidth() / 2.0f;
	float left = -1 * right;

	TextClass *fpsText = new TextClass();
	result = fpsText->Initialize("FPS 0", font, XMFLOAT2(100.0f, top - 50.0f), XMFLOAT2(2.0f, 2.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	if (!result)
		return false;
	m_allText.push_back(fpsText);

	TextClass *playerx = new TextClass();
	result = playerx->Initialize("X 0", font, XMFLOAT2(100.0f, top - 100.0f), XMFLOAT2(2.0f, 2.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	if (!result)
		return false;
	m_allText.push_back(playerx);

	TextClass *playerz = new TextClass();
	result = playerz->Initialize("Z 0", font, XMFLOAT2(100.0f, top - 150.0f), XMFLOAT2(2.0f, 2.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	if (!result)
		return false;
	m_allText.push_back(playerz);

	BitmapClass *bitmap = new BitmapClass();
	result = bitmap->Initialize(seafloor, m_DefaultLightSource, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(100.0f, 100.0f));
	if(!result)
		return false;
	m_allBitmaps.push_back(bitmap);

	return true;
}

bool TwoDGraphicsClass::RenderAll(ShaderControllerClass* shader, int fps)
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
	std::string myfps = "FPS " + std::to_string(fps);
	std::string playerx = "X " + std::to_string(PlayerClass::getInstance()->getPosition().x);
	std::string playerz = "Z " + std::to_string(PlayerClass::getInstance()->getPosition().z);
	m_allText[0]->UpdateText(myfps);
	m_allText[1]->UpdateText(playerx);
	m_allText[2]->UpdateText(playerz);

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
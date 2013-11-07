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

	m_allBitmaps = vector<BitmapClass*>();
	m_allText = map<string, TextClass*>();

	m_DefaultLightSource = new LightClass();

	m_DefaultLightSource->SetAmbientColor(1.0, 1.0, 1.0, 1.0);
	m_DefaultLightSource->SetDiffuseColor(1.0, 1.0, 1.0, 1.0);
	m_DefaultLightSource->SetDirection(0.0, 0.0, -1.0);

	MeshClass *bottommenu = new MeshClass();
	result = bottommenu->Initialize("data/bottommenu.3dmodel", MeshClass::TWOD);
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
	result = fpsText->Initialize("FPS 0", font, XMFLOAT2(right - 150.0f, top - 50.0f), XMFLOAT2(2.0f, 2.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	if (!result)
		return false;
	m_allText["fps"] = fpsText;

	TextClass *playerx = new TextClass();
	result = playerx->Initialize("X 0", font, XMFLOAT2(right - 150.0f, top - 100.0f), XMFLOAT2(2.0f, 2.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	if (!result)
		return false;
	m_allText["playerx"] = playerx;

	TextClass *playerz = new TextClass();
	result = playerz->Initialize("Z 0", font, XMFLOAT2(right - 150.0f, top - 150.0f), XMFLOAT2(2.0f, 2.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	if (!result)
		return false;
	m_allText["playerz"] = playerz;

	TextClass *shields = new TextClass();
	result = shields->Initialize("0/100", font, XMFLOAT2(-300.0f, bottom + 60.0f), XMFLOAT2(4.0f, 4.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	if (!result)
		return false;
	m_allText["shields"] = shields;

	TextClass *health = new TextClass();
	result = health->Initialize("100/100", font, XMFLOAT2(-600.0f, bottom + 60.0f), XMFLOAT2(4.0f, 4.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	if (!result)
		return false;
	m_allText["health"] = health;

	TextClass *torpedos = new TextClass();
	result = torpedos->Initialize("50", font, XMFLOAT2(300.0f, bottom + 60.0f), XMFLOAT2(4.0f, 4.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	if (!result)
		return false;
	m_allText["torpedos"] = torpedos;

	TextClass *energy = new TextClass();
	result = energy->Initialize("100/100", font, XMFLOAT2(0.0f, bottom + 60.0f), XMFLOAT2(4.0f, 4.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	if (!result)
		return false;
	m_allText["energy"] = energy;

	TextClass *thrust = new TextClass();
	result = thrust->Initialize("0/100", font, XMFLOAT2(600.0f, bottom + 60.0f), XMFLOAT2(4.0f, 4.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	if (!thrust)
		return false;
	m_allText["thrust"] = thrust;

	BitmapClass *bottommenu_bitmap = new BitmapClass();
	result = bottommenu_bitmap->Initialize(bottommenu, m_DefaultLightSource, XMFLOAT2(0.0f, bottom + 100.0f), XMFLOAT2(400.0f, 400.0f));
	if(!result)
		return false;
	m_allBitmaps.push_back(bottommenu_bitmap);

	return true;
}

bool TwoDGraphicsClass::RenderAll(ShaderControllerClass* shader, int fps)
{
	bool result;

	shader->Set2DShaders();

	//Turn off the ZBuffer since 2D elements have no Z component
	D3DClass::getInstance()->TurnZBufferOff();
	D3DClass::getInstance()->TurnOnAlphaBlending();

	vector<BitmapClass*>::iterator bitmap;
	for (bitmap = m_allBitmaps.begin(); bitmap != m_allBitmaps.end(); ++bitmap)
	{
		result = (*bitmap)->Render(shader);
		if (!result)
			return false;
	}

	shader->SetTextShaders();

	std::string myfps = "FPS " + std::to_string(fps);
	std::string playerx = "X " + std::to_string(PlayerClass::getInstance()->getPosition().x);
	std::string playerz = "Z " + std::to_string(PlayerClass::getInstance()->getPosition().z);
	std::string sheilds = std::to_string(PlayerClass::getInstance()->GetShields()) +"\\" + std::to_string(PlayerClass::getInstance()->GetTotalShields());
	std::string health = std::to_string(PlayerClass::getInstance()->GetHealth()) +"\\" + std::to_string(PlayerClass::getInstance()->GetTotalHealth());
	std::string torpedos = std::to_string(PlayerClass::getInstance()->GetTorpedos());
	std::string energy = std::to_string(PlayerClass::getInstance()->GetEnergy()) +"\\" + std::to_string(PlayerClass::getInstance()->GetTotalEnergy());
	std::string thrust = std::to_string((int)PlayerClass::getInstance()->GetEnginePower()*100) + "\\100";

	m_allText["fps"]->UpdateText(myfps);
	m_allText["playerx"]->UpdateText(playerx);
	m_allText["playerz"]->UpdateText(playerz);
	m_allText["shields"]->UpdateText(sheilds);
	m_allText["health"]->UpdateText(health);
	m_allText["torpedos"]->UpdateText(torpedos);
	m_allText["energy"]->UpdateText(energy);
	m_allText["thrust"]->UpdateText(thrust);

	map<string, TextClass*>::iterator text;
	for (text = m_allText.begin(); text != m_allText.end(); ++text)
	{
		result = (*text).second->Render(shader);
		if (!result)
			return false;
	}
	D3DClass::getInstance()->TurnOffAlphaBlending();

	//Turn the ZBuffer back on for future 3d rendering
	D3DClass::getInstance()->TurnZBufferOn();

	return true;
}
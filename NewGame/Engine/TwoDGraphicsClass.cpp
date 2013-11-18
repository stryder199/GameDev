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
#include "MeshControllerClass.h"

TwoDGraphicsClass::TwoDGraphicsClass()
{
	m_DefaultLightSource = 0;
}

TwoDGraphicsClass::~TwoDGraphicsClass()
{
}

bool TwoDGraphicsClass::Initialize()
{
	//bool result;
	m_allBitmaps = vector<BitmapClass*>();
	m_allText = map<string, TextClass*>();
	m_allFont = map<string, FontClass*>();

	m_DefaultLightSource = new LightClass();

	m_DefaultLightSource->SetAmbientColor(1.0, 1.0, 1.0, 1.0);
	m_DefaultLightSource->SetDiffuseColor(1.0, 1.0, 1.0, 1.0);
	m_DefaultLightSource->SetDirection(0.0, 0.0, -1.0);

	return true;
}

void TwoDGraphicsClass::Shutdown()
{
	if (m_DefaultLightSource)
	{
		delete m_DefaultLightSource;
		m_DefaultLightSource = 0;
	}

	vector<BitmapClass*>::iterator bitmap;
	for (bitmap = m_allBitmaps.begin(); bitmap != m_allBitmaps.end(); ++bitmap)
	{
		(*bitmap)->Shutdown();
		delete (*bitmap);
		(*bitmap) = 0;
	}

	map<string, TextClass*>::iterator text;
	for (text = m_allText.begin(); text != m_allText.end(); ++text)
	{
		(*text).second->Shutdown();
		delete (*text).second;
		(*text).second = 0;
	}

	map<string, FontClass*>::iterator font;
	for (font = m_allFont.begin(); font != m_allFont.end(); ++font)
	{
		(*font).second->Shutdown();
		delete (*font).second;
		(*font).second = 0;
	}
}

bool TwoDGraphicsClass::RenderAll(ShaderControllerClass* shader, int fps)
{
	bool result;

	shader->Set2DShaders();

	//Turn off the ZBuffer since 2D elements have no Z component
	D3DClass::getInstance()->TurnZBufferOff();
	D3DClass::getInstance()->TurnOnAlphaBlending();

	bitmapMutex.lock();
	vector<BitmapClass*>::iterator bitmap;
	for (bitmap = m_allBitmaps.begin(); bitmap != m_allBitmaps.end(); ++bitmap)
	{
		result = (*bitmap)->PreProcessing();
		if (!result)
			return false;
	}
	bitmapMutex.unlock();

	bitmapMutex.lock();
	for (bitmap = m_allBitmaps.begin(); bitmap != m_allBitmaps.end(); ++bitmap)
	{
		result = (*bitmap)->Render(shader);
		if (!result)
			return false;
	}
	bitmapMutex.unlock();

	shader->SetTextShaders();

	std::string myfps = "FPS " + std::to_string(fps);
	std::string playerx = "X " + std::to_string(PlayerClass::getInstance()->getPosition().x);
	std::string playerz = "Z " + std::to_string(PlayerClass::getInstance()->getPosition().z);
	std::string sheilds = std::to_string(PlayerClass::getInstance()->GetShields()) +"\\" + std::to_string(PlayerClass::getInstance()->GetTotalShields());
	std::string health = std::to_string(PlayerClass::getInstance()->GetHealth()) +"\\" + std::to_string(PlayerClass::getInstance()->GetTotalHealth());
	std::string torpedos = std::to_string(PlayerClass::getInstance()->GetTorpedos());
	std::string energy = std::to_string(PlayerClass::getInstance()->GetEnergy()) +"\\" + std::to_string(PlayerClass::getInstance()->GetTotalEnergy());
	std::string thrust = std::to_string((int)PlayerClass::getInstance()->GetEnginePower()*100) + "\\100";

	textMutex.lock();
	if (m_allText.find("fps") != m_allText.end())
	{
		m_allText["fps"]->UpdateText(myfps);
	}

	if (m_allText.find("playerx") != m_allText.end())
	{
		m_allText["playerx"]->UpdateText(playerx);
	}

	if (m_allText.find("playerz") != m_allText.end())
	{
		m_allText["playerz"]->UpdateText(playerz);
	}

	if (m_allText.find("shields") != m_allText.end())
	{
		m_allText["shields"]->UpdateText(sheilds);
	}

	if (m_allText.find("health") != m_allText.end())
	{
		m_allText["health"]->UpdateText(health);
	}

	if (m_allText.find("torpedos") != m_allText.end())
	{
		m_allText["torpedos"]->UpdateText(torpedos);
	}

	if (m_allText.find("energy") != m_allText.end())
	{
		m_allText["energy"]->UpdateText(energy);
	}

	if (m_allText.find("thrust") != m_allText.end())
	{
		m_allText["thrust"]->UpdateText(thrust);
	}
	textMutex.unlock();

	textMutex.lock();
	map<string, TextClass*>::iterator text;
	for (text = m_allText.begin(); text != m_allText.end(); ++text)
	{
		result = (*text).second->PreProcessing();
		if (!result)
			return false;
	}
	textMutex.unlock();
	
	textMutex.lock();
	for (text = m_allText.begin(); text != m_allText.end(); ++text)
	{
		result = (*text).second->Render(shader);
		if (!result)
			return false;
	}
	textMutex.unlock();
	D3DClass::getInstance()->TurnOffAlphaBlending();

	//Turn the ZBuffer back on for future 3d rendering
	D3DClass::getInstance()->TurnZBufferOn();

	return true;
}

bool TwoDGraphicsClass::AddText(string name, string initText, string fontname, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT4 color)
{
	bool result;
	fontMutex.lock();
	FontClass* font = m_allFont[fontname];
	fontMutex.unlock();
	if (font == NULL)
		return false;

	TextClass *newText = new TextClass();
	result = newText->Initialize(initText, font, pos, scale, color);
	if (!result)
		return false;

	textMutex.lock();
	m_allText[name] = newText;
	textMutex.unlock();

	return true;
}

bool TwoDGraphicsClass::AddBitmap(string meshname, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale)
{
	bool result;
	BitmapClass *bitmap = new BitmapClass();
	MeshClass *mesh = MeshControllerClass::getInstance()->getMesh(meshname);
	if (mesh == NULL)
		return false;

	result = bitmap->Initialize(mesh, m_DefaultLightSource, pos, scale);
	if (!result)
		return false;
	bitmapMutex.lock();
	m_allBitmaps.push_back(bitmap);
	bitmapMutex.unlock();

	return true;
}

bool TwoDGraphicsClass::AddFont(string name, string fontFilename, string textureFilename)
{
	bool result;
	FontClass *newFont = new FontClass();
	result = newFont->Initialize(fontFilename, textureFilename);
	if (!result)
		return false;

	fontMutex.lock();
	m_allFont[name] = newFont;
	fontMutex.unlock();

	return true;
}
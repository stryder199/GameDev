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

void TwoDGraphicsClass::Initialize()
{
	//bool result;
	m_allBitmaps = vector<BitmapClass*>();
	m_allText = map<string, TextClass*>();
	m_allFont = map<string, FontClass*>();

	m_DefaultLightSource = new LightClass();

	m_DefaultLightSource->SetAmbientColor(1.0, 1.0, 1.0, 1.0);
	m_DefaultLightSource->SetDiffuseColor(1.0, 1.0, 1.0, 1.0);
	m_DefaultLightSource->SetDirection(0.0, 0.0, -1.0);
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

void TwoDGraphicsClass::RenderAll(ShaderControllerClass* shader, int fps)
{
	shader->Set2DShaders();

	//Turn off the ZBuffer since 2D elements have no Z component
	D3DClass::getInstance()->TurnZBufferOff();
	D3DClass::getInstance()->TurnOnAlphaBlending();

	bitmapMutex.lock();
	vector<BitmapClass*>::iterator bitmap;
	for (bitmap = m_allBitmaps.begin(); bitmap != m_allBitmaps.end(); ++bitmap)
	{
		(*bitmap)->PreProcessing();
	}
	bitmapMutex.unlock();

	bitmapMutex.lock();
	for (bitmap = m_allBitmaps.begin(); bitmap != m_allBitmaps.end(); ++bitmap)
	{
		(*bitmap)->Render(shader);
	}
	bitmapMutex.unlock();

	shader->SetTextShaders();

	string myfps = "FPS " + to_string(fps);
	string playerx = "X " + to_string(PlayerClass::getInstance()->getPosition().x);
	string playerz = "Z " + to_string(PlayerClass::getInstance()->getPosition().z);
	string sheilds = to_string(PlayerClass::getInstance()->GetShields()) +"\\" + to_string(PlayerClass::getInstance()->GetTotalShields());
	string health = to_string(PlayerClass::getInstance()->GetHealth()) +"\\" + to_string(PlayerClass::getInstance()->GetTotalHealth());
	string torpedos = to_string(PlayerClass::getInstance()->GetTorpedos());
	string energy = to_string(PlayerClass::getInstance()->GetEnergy()) +"\\" + to_string(PlayerClass::getInstance()->GetTotalEnergy());
	string thrust = to_string((int)PlayerClass::getInstance()->GetEnginePower()*100) + "\\100";

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
		(*text).second->PreProcessing();
	}
	textMutex.unlock();
	
	textMutex.lock();
	for (text = m_allText.begin(); text != m_allText.end(); ++text)
	{
		(*text).second->Render(shader);
	}
	textMutex.unlock();
	D3DClass::getInstance()->TurnOffAlphaBlending();

	//Turn the ZBuffer back on for future 3d rendering
	D3DClass::getInstance()->TurnZBufferOn();
}

void TwoDGraphicsClass::AddText(string name, string initText, string fontname, XMFLOAT2 pos, XMFLOAT2 scale, XMFLOAT4 color)
{
	fontMutex.lock();
	FontClass* font = m_allFont[fontname];
	fontMutex.unlock();

	TextClass *newText = new TextClass();
	newText->Initialize(initText, font, pos, scale, color);

	textMutex.lock();
	m_allText[name] = newText;
	textMutex.unlock();
}

void TwoDGraphicsClass::AddBitmap(string meshname, XMFLOAT2 pos, XMFLOAT2 scale)
{
	BitmapClass *bitmap = new BitmapClass();
	MeshClass *mesh = MeshControllerClass::getInstance()->getMesh(meshname);

	bitmap->Initialize(mesh, m_DefaultLightSource, pos, scale);
	bitmapMutex.lock();
	m_allBitmaps.push_back(bitmap);
	bitmapMutex.unlock();
}

void TwoDGraphicsClass::AddFont(string name, string fontFilename, string textureFilename)
{
	FontClass *newFont = new FontClass();
	newFont->Initialize(fontFilename, textureFilename);

	fontMutex.lock();
	m_allFont[name] = newFont;
	fontMutex.unlock();
}
#include "stdafx.h"
#include "Subject.h"
#include "Unit.h"
#include "Observer.h"
#include "Player.h"
#include "UI_Player.h"
#include "PlayerObserver.h"
#include "PlayerData.h"

PlayerData* PlayerData::Create(string name)
{
	PlayerData* temp = new PlayerData();
	temp->play->name = name;
	return temp;
}

PlayerData::PlayerData()
{
	play = Player::Create();
	play->LoadFile("Unit/Man.xml");

	pObserver = new PlayerObserver(play);
	play->Attach(pObserver);

	UI = UI_Player::Create();
	UI->LoadFile("UI/Main_Layout_1.xml");
	UI->SetData(play);
}

void PlayerData::MainCamSet()
{
	Camera::main = static_cast<Camera*>(play->Find("Camera"));
}

void PlayerData::Update()
{
	play->Update();
	UI->Update();
}

void PlayerData::LateUpdate()
{
	play->LateUpdate();
}

void PlayerData::Hierarchy()
{
	play->RenderHierarchy();
	UI->RenderHierarchy();
}

void PlayerData::Render(shared_ptr<Shader> pShader)
{
	UI->Render(pShader);
}

void PlayerData::EffectRender()
{
	play->SpecialEffectsRender();
}

void PlayerData::DeferredRender(shared_ptr<Shader> pShader)
{
	play->Render(pShader);
}

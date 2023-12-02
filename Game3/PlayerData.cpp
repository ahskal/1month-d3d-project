#include "stdafx.h"
#include "Player.h"
#include "UI_Player.h"
#include "PlayerObserver.h"
#include "Inventory.h"
#include "PlayerData.h"

PlayerData* PlayerData::Create(string name)
{
	PlayerData* temp = new PlayerData();
	temp->actor->name = name;
	return temp;
}

PlayerData::PlayerData()
{
	actor = Player::Create();
	actor->LoadFile("Unit/Man.xml");

	pObserver = new PlayerObserver(actor);
	actor->Attach(pObserver);

	UI = UI_Player::Create();
	UI->LoadFile("UI/Main_Layout_1.xml");
	UI->SetData(actor);

	inventory = new Inventory();
}

void PlayerData::MainCamSet()
{
	Camera::main = static_cast<Camera*>(actor->Find("Camera"));
}

void PlayerData::Update()
{
	actor->Update();
	UI->Update();
	inventory->Update();
}

void PlayerData::LateUpdate()
{
	actor->LateUpdate();
}

void PlayerData::Hierarchy()
{
	actor->RenderHierarchy();
	UI->RenderHierarchy();
}

void PlayerData::Render(shared_ptr<Shader> pShader)
{
	UI->Render(pShader);
	inventory->Render(pShader);
}

void PlayerData::DeferredRender(shared_ptr<Shader> pShader)
{
	actor->Render(pShader);
}

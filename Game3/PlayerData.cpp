#include "stdafx.h"
#include "Player.h"
#include "UI_Player.h"
#include "PlayerObserver.h"
#include "PlayerData.h"

PlayerData::PlayerData()
{
	play = Player::Create();
	play->LoadFile("Unit/Man2.xml");
	pObserver = new PlayerObserver(play);
	play->Attach(pObserver);
	UI = UI_Player::Create();
	UI->LoadFile("UI/Hpbar.xml");
	UI->parent = play;
}

void PlayerData::Update()
{
	play->Update();
	UI->Update();
}

void PlayerData::Render()
{
	UI->Render();
}

void PlayerData::DeferredRender(shared_ptr<Shader> pShader)
{
	play->Render(pShader);
}

#include "stdafx.h"
#include "player.h"
#include "UI_Player.h"
extern bool DEBUG_MODE;

UI_Player* UI_Player::Create(string name)
{
	UI_Player* temp = new UI_Player();
	temp->name = name;
	temp->Initialize();
	return temp;
}

UI_Player::UI_Player()
{
	player = nullptr;
}

UI_Player::~UI_Player()
{
}

void UI_Player::Update()
{
	UI::Update();
	if (player != nullptr) {
		auto hp = Find("HpCurrent");
		auto sp = Find("SpCurrent");
		hp->scale.x = (player->Hp / player->MaxHp);
		sp->scale.x = (player->Sp / player->MaxSp);
	}
}

void UI_Player::Render(shared_ptr<Shader> pShader)
{
	UI::Render(pShader);
}

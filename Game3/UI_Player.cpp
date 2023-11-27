#include "stdafx.h"
#include "UI_Player.h"
extern bool DEBUG_MODE;

UI_Player* UI_Player::Create(string name)
{
	UI_Player* temp = new UI_Player();
	temp->name = name;
	temp->type = ObType::UI;
	temp->mesh = RESOURCE->meshes.Load("6.UI.mesh");
	temp->shader = RESOURCE->shaders.Load("6.UI.hlsl");
	return temp;
}

UI_Player::UI_Player()
{
	status.Attack = 0;
	status.Defense = 0;
	status.MaxHp = 0;
	status.Hp = 0;
	status.MaxSp = 0;
	status.Sp = 0;
}

UI_Player::~UI_Player()
{
}

void UI_Player::SetStatus(Status status)
{
	this->status.Hp = status.Hp;
	this->status.MaxHp = status.MaxHp;
	this->status.Sp = status.Sp;
	this->status.MaxSp = status.MaxSp;
	this->status.Attack = status.Attack;
	this->status.Defense = status.Defense;
}

void UI_Player::Update()
{
	UI::Update();
	auto hp = Find("HpCurrent");
	auto sp = Find("SpCurrent");
	hp->scale.x = status.Hp / status.MaxHp;
	sp->scale.x = status.Sp / status.MaxSp;
}

void UI_Player::Render(shared_ptr<Shader> pShader)
{
	UI::Render(pShader);
}

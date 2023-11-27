#include "UI_Monster.h"

UI_Monster* UI_Monster::Create(string name)
{
	UI_Monster* temp = new UI_Monster();
	temp->name = name;
	temp->type = ObType::Billboard;
	temp->mesh = make_shared<Mesh>();
	temp->mesh->LoadFile("7.Billboard.mesh");
	temp->shader = RESOURCE->shaders.Load("7.Billboard.hlsl");
	temp->shader->LoadGeometry();
	return temp;
}

UI_Monster::UI_Monster()
{
	status.Attack = 0;
	status.Defense = 0;
	status.MaxHp = 0;
	status.Hp = 0;
	status.MaxSp = 0;
	status.Sp = 0;
}

UI_Monster::~UI_Monster()
{
}

void UI_Monster::SetStatus(Status status)
{
	this->status.Hp = status.Hp;
	this->status.MaxHp = status.MaxHp;
	this->status.Sp = status.Sp;
	this->status.MaxSp = status.MaxSp;
	this->status.Attack = status.Attack;
	this->status.Defense = status.Defense;
}

void UI_Monster::Update()
{
	ImGui::Text("Hp : %f ", status.Hp);
	Billboard::Update();
	auto hp = Find("HpCurrent");
	hp->scale.x = status.Hp / status.MaxHp * scale.x;
}

void UI_Monster::Render(shared_ptr<Shader> pShader)
{
	Billboard::Render(pShader);
}

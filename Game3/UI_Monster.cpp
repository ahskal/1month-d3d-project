#include "stdafx.h"
#include "Observer.h"
#include "Monster.h"
#include "UI_Monster.h"

UI_Monster* UI_Monster::Create(string name)
{
	UI_Monster* temp = new UI_Monster();
	temp->name = name;	
	temp->type = ObType::Billboard;
	temp->mesh = make_shared<Mesh>();
	temp->mesh->LoadFile("7.Billboard.mesh");
	temp->shader = RESOURCE->shaders.Load("7.Billboard_Deferred.hlsl");
	temp->shader->LoadGeometry();
	return temp;
}

UI_Monster::UI_Monster()
{
	monster = nullptr;
}

UI_Monster::~UI_Monster(){}

void UI_Monster::Update()
{
	Billboard::Update();
	if (monster != nullptr) {
		auto hpCurrent = Find("HpCurrent");
		hpCurrent->scale.x = (monster->Hp / monster->MaxHp) * scale.x;
	}
}

void UI_Monster::Render(shared_ptr<Shader> pShader)
{
	Billboard::Render(pShader);
}

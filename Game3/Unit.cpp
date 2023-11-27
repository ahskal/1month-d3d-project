#include "stdafx.h"
#include "Unit.h"

Unit* Unit::Create(string name)
{
	Unit* temp = new Unit();
	temp->type = ObType::Actor;
	temp->name = name;
	return temp;
}

Unit* Unit::Create(Unit* src)
{
	Unit* temp = new Unit(*src);
	temp->type = ObType::Actor;
	temp->CopyChild(src);
	return temp;
}

Unit::Unit()
	: Hp(0), MaxHp(0), Attack(0), Defense(0)
{
}

Unit::~Unit()
{
}

void Unit::Update()
{
	Actor::Update();
}

void Unit::Render(shared_ptr<Shader> pShader)
{
	Actor::Render(pShader);
}

void Unit::RenderDetail()
{
	Actor::RenderDetail();
}

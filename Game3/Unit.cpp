#include "stdafx.h"
#include "Unit.h"

Unit* Unit::Create(string name)
{
	Unit* temp = new Unit;
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

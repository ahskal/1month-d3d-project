#include "stdafx.h"
#include "Unit.h"

Unit* Unit::Create(string name)
{
	Unit* temp = new Unit();
	temp->type = ObType::Actor;
	temp->name = name;
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

	if (hitCooltime > 0.0f)
	{
		hitCooltime -= DELTA;
	}
	else {
		hit = false;
	}
	// CurHp가 0 이하로 내려가지 않도록 보정
	if (Hp < 0) Hp = 0;
	// CurHp가 MaxHp를 넘지 않도록 보정
	if (Hp > MaxHp) Hp = MaxHp;
	// 이미지 업데이트

}

void Unit::Render(shared_ptr<Shader> pShader)
{
	Actor::Render(pShader);
}

void Unit::RenderDetail()
{
	Actor::RenderDetail();
}

void Unit::Damage(int damage)
{
	if (hitCooltime <= 0.0f)
	{
		Hp -= damage;
		hitCooltime = 0.05f;
		//cout << this->GetWorldPos().x<< this->GetWorldPos().y << endl;
		//DAMAGE->AddText(this->GetWorldPos() + Vector2(50, 50), damage, color);
		hit = true;
	}
}

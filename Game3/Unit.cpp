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
	: Hp(0), MaxHp(0), Attack(0), Defense(0), hitCooltime(0.0f), hit(false), isAttack(false)
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
	// CurHp�� 0 ���Ϸ� �������� �ʵ��� ����
	if (Hp < 0) Hp = 0;
	// CurHp�� MaxHp�� ���� �ʵ��� ����
	if (Hp > MaxHp) Hp = MaxHp;
	// �̹��� ������Ʈ

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
		DAMAGEFONT->AddDamageText(damage, GetWorldPos());

		Hp -= damage;
		hitCooltime = 0.1f;
		//cout << this->GetWorldPos().x<< this->GetWorldPos().y << endl;
		//DAMAGE->AddText(this->GetWorldPos() + Vector2(50, 50), damage, color);
		hit = true;
	}
}

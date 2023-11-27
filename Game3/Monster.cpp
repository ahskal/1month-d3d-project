#include "stdafx.h"
#include "Subject.h"
#include "Monster.h"

extern bool DEBUG_MODE;
using namespace Mon;

Monster* Monster::Create(string name)
{
	Monster* temp = new Monster;
	temp->LoadFile("Unit/Man2.xml");
	temp->type = ObType::Actor;
	temp->state = State::IDLE;
	temp->name = name;
	return temp;
}

Monster* Monster::Create(Monster* src)
{
	Monster* temp = new Monster(*src);
	temp->type = ObType::Actor;
	temp->LoadFile("Unit2.xml");
	temp->CopyChild(src);
	return temp;
}

Monster::Monster()
{


}

Monster::~Monster()
{
}

void Monster::SetSpawn(Vector3 spawn)
{
	SetWorldPosX(spawn.x);
	SetWorldPosZ(spawn.z);
}

void Monster::Init()
{
	anim->ChangeAnimation(AnimationState::LOOP, Ani_Idle_Eqip, 0.1f);
	slash = new SlashTrail();
	slash->Top = Find("Start");
	slash->Bottom = Find("End");

	slash->interval = 0.004f;
	slash->maxTrail = 50;
	slash->material->diffuseMap = RESOURCE->textures.Load("trail.png");
	slash->material->diffuse.w = 1.0f;

	slash->material->diffuse.x = 0.0f;
	slash->material->diffuse.y = 0.0f;
	slash->material->diffuse.z = 1.0f;

	MaxHp = Hp = 200;
	Attack = 10;
	Defense = 5;

	anim->aniScale = 0.6f;
	MoveSpeed = 3;

	Length = 10;
}

void Monster::Update()
{
	if (Once) {
		Init();
		Once = false;
	}
	FSM();
	Unit::Update();
}

void Monster::LateUpdate()
{


}

void Monster::Render(shared_ptr<Shader> pShader)
{
	Unit::Render(pShader);

}

void Monster::SpecialEffectsRender()
{
}

void Monster::FSM()
{
	if (state == State::IDLE) {
		if (IsInRadius()) {
			state = State::WALK;
			anim->ChangeAnimation(AnimationState::LOOP, Ani_Move_Eqip_Front, 0.1f);
		}
	}
	else if (state == State::WALK) {

		Vector3 dir = target - GetWorldPos();
		float dis = dir.Length();
		if (dis < 3) {
			state = State::ATTACK;
			anim->ChangeAnimation(AnimationState::ONCE_LAST, Ani_Attack_02, 0.1f);
			return;
		}
		else if (dis > Length) {
			state = State::IDLE;
			anim->ChangeAnimation(AnimationState::LOOP, Ani_Idle_Eqip, 0.1f);
			return;
		}		
		dir.Normalize();
		rotation.y = atan2f(dir.x, dir.z);
		MoveWorldPos(dir * DELTA * MoveSpeed);
	}
	else if (state == State::ATTACK) {

		if (anim->GetPlayTime() >= 0.98f) {
			state = State::IDLE;
		}
	}
	else if (state == State::GUARD) {

	}
	else if (state == State::DAMAGE) {

	}
	else if (state == State::DEAD) {

	}

}

void Monster::Move()
{
}

void Monster::Hierarchy()
{
	RenderHierarchy();

}

bool Monster::IsInRadius()
{
	Vector3 dis = target - GetWorldPos();
	if (dis.Length() < Length) {
		return true;
	}
	return false;
}

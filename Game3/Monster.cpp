#include "stdafx.h"
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
	MoveSpeed = 5;

	Eqip[0] = Ani_Move_Eqip_Left;
	Eqip[1] = Ani_Move_Eqip_Back_Right;
	Eqip[2] = Ani_Move_Eqip_Back;
	Eqip[3] = Ani_Move_Eqip_Back_Left;
	Eqip[4] = Ani_Move_Eqip_Right;
	Eqip[5] = Ani_Move_Eqip_Front_Right;
	Eqip[6] = Ani_Move_Eqip_Front;
	Eqip[7] = Ani_Move_Eqip_Front_Left;
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

	UpdateObserver();
}

void Monster::Update()
{
	if (Once) {
		Init();
		Once = false;
	}

	

	Unit::Update();
	UpdateObserver();
}

void Monster::LateUpdate()
{
	
	if (state == State::IDLE) {

	}


}

void Monster::Render(shared_ptr<Shader> pShader)
{
	Unit::Render(pShader);
	
}

void Monster::LateRender()
{
}

void Monster::FSM()
{
}

void Monster::Move()
{
}

void Monster::Hierarchy()
{
	RenderHierarchy();

}

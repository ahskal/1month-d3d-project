#include "stdafx.h"
#include "Subject.h"
#include "Monster.h"


extern bool DEBUG_MODE;
using namespace Mon;

Monster* Monster::Create(string name)
{
	Monster* temp = new Monster();
	temp->LoadFile("Unit/Man2.xml");
	temp->type = ObType::Actor;
	temp->state = State::IDLE;
	temp->name = name;
	temp->observerName = name;
	return temp;
}

Monster::Monster(){
	Length = 10;
	moveDir = Vector3();
	index = 0;
	state = State::IDLE;
	MoveSpeed = 3;
	slash = nullptr;

}

Monster::~Monster(){}

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

}

void Monster::Update()
{
	if (Once) {
		Init();
		Once = false;
	}
	FSM();
	lastPos = GetWorldPos();
	Unit::Update();
}

void Monster::Render(shared_ptr<Shader> pShader)
{
	Unit::Render(pShader);
	slash->Render();
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
		Move();		
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

bool Monster::IsInRadius()
{
	Vector3 dis = target - GetWorldPos();
	if (dis.Length() < Length) {
		return true;
	}
	return false;
}

void Monster::WolrdUpdate()
{
	GameObject::Update();
}

void Monster::GoBack()
{
	SetWorldPos(lastPos);
	WolrdUpdate();
}

void Monster::Move()
{
	moveDir = target - GetWorldPos();
	float dis = moveDir.Length();
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
	moveDir.Normalize();
	rotation.y = atan2f(moveDir.x, moveDir.z);
	MoveWorldPos(moveDir * DELTA * MoveSpeed);
}

void Monster::Hierarchy()
{
	RenderHierarchy();
}

void Monster::Update(const std::string& message)
{
	if (DEBUG_MODE) {
		float WTime = TIMER->GetWorldTime();
		cout << "Observer " << observerName << endl;
		cout << "[" << std::fixed << std::setprecision(2) << WTime << "]" << " messageCall : "
			<< message << endl;
	}
}

void Monster::Update(const Vector3& position)
{
	target = position;
	if (DEBUG_MODE) {
		float WTime = TIMER->GetWorldTime();
		cout << "Observer " << observerName << endl;
		cout << "[" << std::fixed << std::setprecision(2) << WTime << "]" << " messageCall : "
			<< position.x << "," << position.y << "," << position.z << endl;
	}	
}

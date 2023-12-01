#include "stdafx.h"
#include "Subject.h"
#include "Player.h"

extern bool DEBUG_MODE;
using namespace play;

Player* Player::Create(string name)
{
	Player* temp = new Player();
	temp->LoadFile("Unit/Man.xml");
	temp->type = ObType::Actor;
	temp->state = State::IDLE;
	temp->name = name;
	return temp;
}


Player::Player()
{
	Laststate = State::IDLE;
	state = State::IDLE;
	MaxSp = 0;
	Sp = 0;
	Money = 0;
	index = 0;
	slash = nullptr;

	MoveSpeed = 5;

	Uneqip[0] = Ani_Move_Uneqip_Left;
	Uneqip[1] = Ani_Move_Uneqip_Back_Right;
	Uneqip[2] = Ani_Move_Uneqip_Back;
	Uneqip[3] = Ani_Move_Uneqip_Back_Left;
	Uneqip[4] = Ani_Move_Uneqip_Right;
	Uneqip[5] = Ani_Move_Uneqip_Front_Right;
	Uneqip[6] = Ani_Move_Uneqip_Front;
	Uneqip[7] = Ani_Move_Uneqip_Front_Left;

	Eqip[0] = Ani_Move_Eqip_Left;
	Eqip[1] = Ani_Move_Eqip_Back_Right;
	Eqip[2] = Ani_Move_Eqip_Back;
	Eqip[3] = Ani_Move_Eqip_Back_Left;
	Eqip[4] = Ani_Move_Eqip_Right;
	Eqip[5] = Ani_Move_Eqip_Front_Right;
	Eqip[6] = Ani_Move_Eqip_Front;
	Eqip[7] = Ani_Move_Eqip_Front_Left;

	lastPos = Vector3();

	isEqip = true;
}

Player::~Player()
{
}

void Player::SetSpawn(Vector3 spawn)
{
	SetWorldPosX(spawn.x);
	SetWorldPosZ(spawn.z);
	lastPos = spawn;
}

void Player::Init()
{
	anim->ChangeAnimation(AnimationState::LOOP, Ani_Idle_Eqip, 0.1f);

	slash = new SlashTrail();
	slash->Top = Find("Start");
	slash->Bottom = Find("End");

	slash->interval = 0.0f;
	slash->maxTrail = 25;
	slash->material->diffuseMap = RESOURCE->textures.Load("trail.png");
	slash->material->diffuse.w = 1.0f;

	slash->material->diffuse.x = 0.0f;
	slash->material->diffuse.y = 0.0f;
	slash->material->diffuse.z = 1.0f;

	MaxHp = Hp = 200;
	MaxSp = Sp = 150;
	Attack = 10;
	Defense = 5;
}

void Player::Update()
{
	
	FSM();

	if (Sp <= MaxSp) {
		Sp += DELTA * 5;
	}

	if (Once) {
		Init();
		Once = false;
	}
	//# Update;
	if (not DEBUG_MODE)
		CameraHold();
	else{
		slash->RenderDetail();
	}

	lastPos = GetWorldPos();
	Actor::Update();
	slash->Update();

}

void Player::FSM()
{
	if (state == State::IDLE or state == State::WALK) {
		Control();
		ChangeAni();
	}

	// IDLE
	if (state == State::IDLE) {
		MoveSpeed = 7;
		if (INPUT->KeyPress('W') or INPUT->KeyPress('A')
			or INPUT->KeyPress('S') or INPUT->KeyPress('D')) {
			state = State::WALK;
		}

		// 무기 장비 해체
		if (INPUT->KeyDown(VK_LCONTROL)) {
			anim->ChangeAnimation(AnimationState::ONCE_LAST, Ani_Equip, 0.1f);
			state = State::UNEQIP;
			shouldOnce = false;
		}

		if (INPUT->KeyDown(VK_LBUTTON) && Sp > 0 && isEqip) {
			Attacker();
		}
	}

	// WALK
	else if (state == State::WALK) {
		MoveSpeed = 5;
		if (INPUT->KeyDown(VK_LBUTTON) && Sp > 0 && isEqip) {
			Attacker();
		}
		if (not(INPUT->KeyPress('W') or INPUT->KeyPress('A')
			or INPUT->KeyPress('S') or INPUT->KeyPress('D')))
		{
			state = State::IDLE;
			if (isEqip) {
				anim->ChangeAnimation(AnimationState::LOOP, Ani_Idle_Eqip, 0.1f);
			}
			if (!isEqip) {
				anim->ChangeAnimation(AnimationState::LOOP, Ani_Idle_UnEqip, 0.1f);
			}
			
		}
	}

	// UNEQIP
	else if (state == State::UNEQIP) {
		if (anim->GetPlayTime() >= 0.35f and not shouldOnce) {
			isEqip = !isEqip;
			shouldOnce = true;
		}
		if (anim->GetPlayTime() >= 0.98f) {
			state = State::IDLE;
			if (isEqip) {
				anim->ChangeAnimation(AnimationState::LOOP, Ani_Idle_Eqip, 0.1f);
			}
			if (!isEqip) {
				anim->ChangeAnimation(AnimationState::LOOP, Ani_Idle_UnEqip, 0.1f);
			}
		}
		if (isEqip) {
			Find("sword")->visible = true;
			Find("shield")->visible = true;
		}
		else {
			Find("sword")->visible = false;
			Find("shield")->visible = false;
		}
	}
	// ATTACK
	else if (state == State::ATTACK) {
		if (slash->isPlaying == false and anim->GetPlayTime() >= 0.1f)
		{
			slash->Play();
		}
		if (slash->isPlaying == true and anim->GetPlayTime() >= 0.8f)
		{
			slash->Stop();
		}
		if (anim->GetPlayTime() >= 0.98f) {
			state = State::IDLE;
			if (Laststate == State::WALK) {
				anim->ChangeAnimation(AnimationState::LOOP, Eqip[index], 0.1f);
				return;
			}
			if (isEqip) {
				anim->ChangeAnimation(AnimationState::LOOP, Ani_Idle_Eqip, 0.1f);
			}
		}
	}
}

void Player::LateUpdate()
{

}

void Player::Render(shared_ptr<Shader> pShader)
{
	Actor::Render(pShader);
	slash->Render();
}

void Player::ChangeAni() {

	if (MoveDir == Vector3())
	{
		index = -1;
		return;
	}
	float dotAngle = GetForward().Dot(MoveDir);
	float dotAngle2 = GetRight().Dot(MoveDir);

	int index = round((atan2f(dotAngle, dotAngle2) + PI) / (45.f * ToRadian));
	index = (index == 8) ? 0 : index;
	
	//ImGui::Text("index %d", index);
	//ImGui::Text("this->index %d", this->index);

	if (this->index != index) {
		if (isEqip) {
			anim->ChangeAnimation(AnimationState::LOOP, Eqip[index], 0.1f);
		}
		else {
			anim->ChangeAnimation(AnimationState::LOOP, Uneqip[index], 0.1f);
		}
	}
	this->index = index;
}

void Player::Control()
{
	bool keyW = INPUT->KeyPress('W');
	bool keyS = INPUT->KeyPress('S');
	bool keyA = INPUT->KeyPress('A');
	bool keyD = INPUT->KeyPress('D');
	MoveDir = Vector3(0, 0, 0);

	if (keyW)
		MoveDir += GetForward();
	else if (keyS)
		MoveDir -= GetForward();
	if (keyA)
		MoveDir -= GetRight();
	else if (keyD)
		MoveDir += GetRight();

	MoveDir.Normalize();
	MoveWorldPos(MoveDir * DELTA * MoveSpeed);

}

void Player::CameraHold()
{
	//중앙값
	POINT ptMouse;
	ptMouse.x = App.GetHalfWidth();
	ptMouse.y = App.GetHalfHeight();
	Vector3 Rot;
	Rot.x = (INPUT->position.y - ptMouse.y) * 0.001f;
	Rot.y = (INPUT->position.x - ptMouse.x) * 0.001f;
	rotation.y += Rot.y;
	Find("Camera")->rotation.x += Rot.x;
	ClientToScreen(App.GetHandle(), &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void Player::Attacker()
{
	isHit = false;
	Laststate = state;
	state = State::ATTACK;
	if (AttackCount == 0) {
		anim->ChangeAnimation(AnimationState::ONCE_LAST, Ani_Attack_02, 0.1f);
		AttackCount = 1;
	}
	else if (AttackCount == 1) {
		anim->ChangeAnimation(AnimationState::ONCE_LAST, Ani_Attack_03, 0.1f);
		AttackCount = 0;
	}
	SetState("공격");
	Sp -= 10;
}

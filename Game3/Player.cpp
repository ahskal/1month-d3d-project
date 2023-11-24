#include "stdafx.h"
#include "Player.h"

Player* Player::Create(string name)
{
	Player* temp = new Player();
	temp->LoadFile("Man2.xml");
	temp->type = ObType::Actor;
	temp->state = PlayerState::IDLE;

	Camera* Cam = Camera::Create();
	Cam->LoadFile("Cam2.xml");

	Cam->SetLocalPosY(3.5);
	Cam->SetLocalPosZ(-5);
	temp->lastPos = Vector3();
	temp->AddChild(Cam);
	Camera::main = Cam;
	temp->Cam = Cam;
	return temp;
}

Player* Player::Create(Player* src)
{
	Player* temp = new Player(*src);
	temp->type = ObType::Actor;
	temp->state = PlayerState::IDLE;
	temp->rotTime = 0.0f;
	temp->CopyChild(src);

	Camera* Cam = Camera::Create();
	Cam->LoadFile("Cam2.xml");

	temp->AddChild(Cam);
	Camera::main = Cam;

	temp->Cam = Cam;
	return temp;
}

Player::Player()
{
	MoveSpeed = 5;


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

	dir[0] = Ani_Move_Left;
	dir[1] = Ani_Move_Back_Right;
	dir[2] = Ani_Move_Back;
	dir[3] = Ani_Move_Back_Left;
	dir[4] = Ani_Move_Right;
	dir[5] = Ani_Move_Front_Right;
	dir[6] = Ani_Move_Front;
	dir[7] = Ani_Move_Front_Left;

	anim->ChangeAnimation(AnimationState::LOOP, Ani_Idle_Equip, 0.1f);

}

void Player::Update()
{
	//rotation.y = Cam->rotation.y - PI;
	Cam->Find("None")->rotation.y = rotation.y - PI;
	lastPos = GetWorldPos();
	Actor::Update();

	if (Once) {
		Init();
		Once = false;
	}

	if (state == PlayerState::IDLE or state == PlayerState::WALK)
		Control();

	// 기본모션
	if (state == PlayerState::IDLE) {


		if (INPUT->KeyPress('W') or INPUT->KeyPress('A')
			or INPUT->KeyPress('S') or INPUT->KeyPress('D'))
		{
			state = PlayerState::WALK;
		}

		// 무기 장비 해체
		if (INPUT->KeyDown(VK_LCONTROL)) {

			anim->ChangeAnimation(AnimationState::ONCE_LAST, Ani_Equip, 0.1f);
			state = PlayerState::UNEQIP;
			shouldOnce = false;
		}

		if (INPUT->KeyDown(VK_LBUTTON)) {
			state = PlayerState::ATTACK;

			if (AttackCount == 0) {
				anim->ChangeAnimation(AnimationState::ONCE_LAST, Ani_Attack_02, 0.1f);

				AttackCount = 1;
			}
			else if (AttackCount == 1) {
				anim->ChangeAnimation(AnimationState::ONCE_LAST, Ani_Attack_03, 0.1f);

				AttackCount = 0;
			}
		}
	}
	// 걷기
	else if (state == PlayerState::WALK) {

		if (INPUT->KeyDown(VK_LBUTTON)) {
			state = PlayerState::ATTACK;

			if (AttackCount == 0) {
				anim->ChangeAnimation(AnimationState::ONCE_LAST, Ani_Attack_02, 0.1f);

				AttackCount = 1;
			}
			else if (AttackCount == 1) {
				anim->ChangeAnimation(AnimationState::ONCE_LAST, Ani_Attack_03, 0.1f);

				AttackCount = 0;
			}
		}
		if (not(INPUT->KeyPress('W') or INPUT->KeyPress('A')
			or INPUT->KeyPress('S') or INPUT->KeyPress('D')))
		{
			state = PlayerState::IDLE;
			if (isUnEquip) {
				anim->ChangeAnimation(AnimationState::LOOP, Ani_Idle_UnEquip, 0.1f);
			}
			if (!isUnEquip) {
				anim->ChangeAnimation(AnimationState::LOOP, Ani_Idle_Equip, 0.1f);
			}
		}


	}
	// 무기 장비 해체
	else if (state == PlayerState::UNEQIP) {
		if (anim->GetPlayTime() >= 0.35f and not shouldOnce) {
			isUnEquip = !isUnEquip;
			shouldOnce = true;
		}
		if (anim->GetPlayTime() >= 0.98f) {
			state = PlayerState::IDLE;
			if (isUnEquip) {
				anim->ChangeAnimation(AnimationState::LOOP, Ani_Idle_UnEquip, 0.1f);
			}
			if (!isUnEquip) {
				anim->ChangeAnimation(AnimationState::LOOP, Ani_Idle_Equip, 0.1f);
			}
		}
		if (isUnEquip) {
			Find("sword")->visible = false;
			Find("shield")->visible = false;
		}
		else {
			Find("sword")->visible = true;
			Find("shield")->visible = true;
		}
	}
	// 공격
	else if (state == PlayerState::ATTACK) {
		if (slash->isPlaying == false and anim->GetPlayTime() >= 0.1f)
		{
			slash->Play();
		}
		if (slash->isPlaying == true and anim->GetPlayTime() >= 0.8f)
		{
			slash->Stop();
		}
		if (anim->GetPlayTime() >= 0.98f) {
			slash->Update();
			state = PlayerState::IDLE;
			if (isUnEquip) {
				anim->ChangeAnimation(AnimationState::LOOP, Ani_Idle_UnEquip, 0.1f);
			}
			if (!isUnEquip) {
				anim->ChangeAnimation(AnimationState::LOOP, Ani_Idle_Equip, 0.1f);
			}
		}
	}

	slash->Update();

}

void Player::LateUpdate()
{
}

void Player::Render(shared_ptr<Shader> pShader)
{
	Actor::Render(pShader);

}

void Player::EffectRender()
{
	slash->Render();
}


void Player::Hierarchy()
{
	Actor::RenderHierarchy();
}

void Player::Control()
{
	bool keyW = INPUT->KeyPress('W');
	bool keyS = INPUT->KeyPress('S');
	bool keyA = INPUT->KeyPress('A');
	bool keyD = INPUT->KeyPress('D');
	if (not moving) {
		return;
	}
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

	float dotAngle = GetForward().Dot(MoveDir);
	float dotAngle2 = GetRight().Dot(MoveDir);

	int index = round((atan2f(dotAngle, dotAngle2) + PI) / (45.f * ToRadian));
	index = (index == 8) ? 0 : index;
	this->index = index;

	if (lastMoveDir != MoveDir) {
		anim->ChangeAnimation(AnimationState::LOOP, dir[index], 0.1f);
	}

	lastMoveDir = MoveDir;
}
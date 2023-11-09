#include "stdafx.h"
#include "Player.h"

Player* Player::Create(string name)
{
	Player* temp = new Player();
	temp->LoadFile("Man2.xml");
	temp->type = ObType::Actor;
	temp->state = PlayerState::IDLE;
	return temp;
}

Player* Player::Create(Player* src)
{
	Player* temp = new Player(*src);
	temp->type = ObType::Actor;
	temp->state = PlayerState::IDLE;
	temp->rotTime = 0.0f;
	temp->CopyChild(src);

	return temp;
}

Player::Player()
{
	MoveSpeed = 5;
}

Player::~Player()
{
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
	slash->RenderDetail();

	slash->Update();
	Actor::Update();

}

void Player::PreRender()
{
	Actor::Render();
	slash->Render();
}

void Player::Render(shared_ptr<Shader> pShader)
{
	Actor::Render(pShader);
	slash->Render();
}


void Player::Hierarchy()
{
	Actor::RenderHierarchy();
}

void Player::Control()
{
	if (!INPUT->KeyPress(VK_LSHIFT)) {
		return;
	}
	MoveDir = Vector2(0,0);

	if (INPUT->KeyPress('W'))
	{
		MoveDir.y = 1.0f;
	}
	else if (INPUT->KeyPress('S'))
	{
		MoveDir.y = -1.0f;
	}
	if (INPUT->KeyPress('A'))
	{
		MoveDir.x = -1.0f;
	}
	else if (INPUT->KeyPress('D'))
	{
		MoveDir.x = 1.0f;
	}
	//MoveDir.Normalize();

	MoveWorldPos(Vector3(MoveDir.x, 0, MoveDir.y) * DELTA * MoveSpeed);

	int index =
		round((atan2f(MoveDir.y, MoveDir.x) + PI) / (45.f * ToRadian));

	if (index == 8) index = 0;
	this->index = index;

	if (lastMoveDir != MoveDir) {
		anim->ChangeAnimation(AnimationState::LOOP, dir[index], 0.2f);
	}

	lastMoveDir = MoveDir;

}
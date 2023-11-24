#pragma once

enum class PlayerState
{
	IDLE,
	WALK,
	UNEQIP,
	ATTACK
};

enum AniState {
	// 0 ~ 7 move
	Ani_Move_Front,
	Ani_Move_Back,
	Ani_Move_Left,
	Ani_Move_Right,
	Ani_Move_Front_Right,
	Ani_Move_Front_Left,
	Ani_Move_Back_Right,
	Ani_Move_Back_Left,

	// 8 swap Weapon
	Ani_Equip,

	// 9 ~ 10 Attack 1,2;
	Ani_Attack_02,
	Ani_Attack_03,

	// 11 ~ 12 Idle Equip, unEquip
	Ani_Idle_Equip,
	Ani_Idle_UnEquip,

};

class Player : public Actor
{
public:

	friend class Scene2;


	static Player* Create(string name = "Player");
	static Player* Create(Player* src);
private:

	Camera* Cam;

	// 플레이어 FSM
	PlayerState state;
	// 키 입력에 따른 방향값
	Vector3 MoveDir;
	Vector3 lastMoveDir;
	// 이전에 키 입력에 따른 방향값
	Vector3 lastPos = Vector3();
	// 방향값을 정수로 변환
	int index;
	// 정수방향값을 애니메이션 순서에 맞게 정렬
	int dir[8];

	// 장비 해체 여부
	bool isUnEquip = false;
	// 장비 해체를 한번만 실행 불린변수
	bool shouldOnce = false;

	//이동속도
	float	MoveSpeed;

	// 공격 모션
	int AttackCount = 0;
	// 검 이동방향에 따른 검기
	SlashTrail* slash;

	bool Once = true;


	float	speed;
	float	time;

	float   rotSpeed;
	float   rotTime;

	Ray attackRay;

	float CamLength = 5;

public:
	bool moving = true;;

	const Vector3& GetDir() { return MoveDir; }


	Player();
	~Player();
	void SetSpawn(Vector3 spawn);
	void Init();
	void Update();
	void LateUpdate();
	void PreRender();
	void Render(shared_ptr<Shader> pShader = nullptr) override;
	void EffectRender();

	void Hierarchy();

	void Control();
	void WolrdUpdate()
	{
		GameObject::Update();
	}

	void GoBack() {
		SetWorldPos(lastPos);
		Update();
	}


};


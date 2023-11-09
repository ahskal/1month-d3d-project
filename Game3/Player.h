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
	static Player* Create(string name = "Player");
	static Player* Create(Player* src);
private:
	// �÷��̾� FSM
	PlayerState state;
	// Ű �Է¿� ���� ���Ⱚ
	Vector2 MoveDir;
	// ������ Ű �Է¿� ���� ���Ⱚ
	Vector2 lastMoveDir;
	// ���Ⱚ�� ������ ��ȯ
	int index;
	// �������Ⱚ�� �ִϸ��̼� ������ �°� ����
	int dir[8];
	
	// ��� ��ü ����
	bool isUnEquip = false;
	// ��� ��ü�� �ѹ��� ���� �Ҹ�����
	bool shouldOnce = false;
	
	//�̵��ӵ�
	float	MoveSpeed;

	// ���� ���
	int AttackCount = 0;
	// �� �̵����⿡ ���� �˱�
	SlashTrail* slash;

	bool Once = true;


	float	speed;
	float	time;

	float   rotSpeed;
	float   rotTime;

	Vector3 last;



public:




	Player();
	~Player();

	void Init();
	void Update();
	void PreRender();
	void Render(shared_ptr<Shader> pShader = nullptr) override;
	void Hierarchy();

	void Control();
	void WolrdUpdate()
	{
		GameObject::Update();
	}


};


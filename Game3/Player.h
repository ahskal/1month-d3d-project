#pragma once
namespace play {
	enum class State
	{
		IDLE,
		WALK,
		RUN,
		UNEQIP,
		ATTACK,
		DAMAGE,
		GUARD,
		PICKUP,
		DEAD,
	};
	enum AniState {
		// 0 ~ 7 Uneqip Move
		Ani_Move_Eqip_Front,
		Ani_Move_Eqip_Back,
		Ani_Move_Eqip_Left,
		Ani_Move_Eqip_Right,
		Ani_Move_Eqip_Front_Right,
		Ani_Move_Eqip_Front_Left,
		Ani_Move_Eqip_Back_Right,
		Ani_Move_Eqip_Back_Left,

		// 8 ~ 15 Eqip Move 
		Ani_Move_Uneqip_Front,
		Ani_Move_Uneqip_Back,
		Ani_Move_Uneqip_Left,
		Ani_Move_Uneqip_Right,
		Ani_Move_Uneqip_Front_Right,
		Ani_Move_Uneqip_Front_Left,
		Ani_Move_Uneqip_Back_Right,
		Ani_Move_Uneqip_Back_Left,

		// 16 ~ 17 Run
		Run_Eqip,
		Run_UnEqip,

		// 18 swap Weapon
		Ani_Equip,
		// 19 Item PickUp
		Ani_PickUp,

		// 20 ~ 21 Idle Equip, unEquip
		Ani_Idle_Eqip,
		Ani_Idle_UnEqip,

		// 22 ~ 25 Block
		Ani_Block,
		Ani_Block_Loop,
		Ani_Block_Guard,
		Ani_Block_Damage,

		// 26 ~ 31 damage
		Ani_Damage_01,
		Ani_Damage_02,
		Ani_Damage_Left_01,
		Ani_Damage_Left_02,
		Ani_Damage_Right_01,
		Ani_Damage_Right_02,

		// 32 ~ 49 Attack
		Ani_Attack_01,
		Ani_Attack_02,
		Ani_Attack_03,
		Ani_Attack_04,
		Ani_Attack_05,
		Ani_Attack_06,
		Ani_Attack_07,
		Ani_Attack_08,
		Ani_Attack_09,
		Ani_Attack_10,
		Ani_Attack_11,
		Ani_Attack_12,
		Ani_Attack_13,
		Ani_Attack_14,
		Ani_Attack_15,
		Ani_Attack_16,
		Ani_Attack_17,
		Ani_Attack_18,

		// 50 ~ 53 Attack
		Ani_Dead_01,
		Ani_Dead_02,
		Ani_Dead_03,
		Ani_Dead_04,
	};
}

class Player : public Subject
{
public:
	friend class Scene2;

	static Player* Create(string name = "Player");
private:
	std::vector<class Item> inventory;
	float Money;

	// �÷��̾� FSM
	play::State state;
	play::State Laststate;

	// Ű �Է¿� ���� ���Ⱚ
	Vector3 MoveDir;

	//�÷��̾��� ������ǥ
	Vector3 lastPos;

	// ���Ⱚ�� ������ ��ȯ
	int index;
	// �������Ⱚ�� �ִϸ��̼� ������ �°� ����
	int Eqip[8];
	int Uneqip[8];

	// ��� ��ü ����
	bool isEqip = false;
	// ��� ��ü�� �ѹ��� ���� �Ҹ�����
	bool shouldOnce = false;

	//�̵��ӵ�
	float	MoveSpeed;

	// ���� ���
	int AttackCount = 0;
	// �� �̵����⿡ ���� �˱�
	SlashTrail* slash;


public:
	//STATUS
	// HP�� �θ�Ŭ�������� ����
	// Attack�� �θ�Ŭ�������� ����
	// Defense�� �θ�Ŭ�������� ����
	// SP�� ���׹̳�
	float Sp, MaxSp;

	bool isHit = true;

	Player();
	~Player();

	void SetSpawn(Vector3 spawn);
	void Init();

	void Update();
	void LateUpdate();

	void Render(shared_ptr<Shader> pShader = nullptr) override;

	void FSM();
	void Control();
	void ChangeAni();


	// ��ǥ ������Ʈ
	void WolrdUpdate()
	{
		GameObject::Update();
	}

	// �浹�� ���� ��ǥ�� �̵�
	void GoBack() {
		SetWorldPos(lastPos);
		WolrdUpdate();
	}

	void CameraHold(); 
	void Attacker();
};


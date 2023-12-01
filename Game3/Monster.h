#pragma once

namespace Mon {
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
		DEADSTART,
		DEADEND,
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
class Monster : public Unit, public Observer
{
public:
	static Monster* Create(string name = "Monster");
private:
	string observerName;

	Mon::State state;

	float	MoveSpeed;
	int AttackCount = 0;
	SlashTrail* slash;
	bool Once = true;

	Vector3 target;

	float Length;

	int index;

	Vector3 lastPos;
	Vector3 moveDir;


public:
	bool DeadAni;
	bool isDead;

	Monster();
	~Monster();

	void SetSpawn(Vector3 spawn);
	void Init();

	void Update();
	void Render(shared_ptr<Shader> pShader = nullptr);


	void FSM();
	void Move();
	void Hierarchy();
	bool IsInRadius();

	void GetTargerPos(Vector3& pos) { target = pos; }
	Mon::State GetState() const { return state; }
	void SetState(Mon::State state) { this->state = state; }

	void SlashOFF() { slash->Stop(); }
	// 좌표 업데이트
	void WolrdUpdate();

	// 충돌시 이전 좌표로 이동
	void GoBack();

	// Observer을(를) 통해 상속됨
	void Update(const string& message);
	void Update(const Vector3& position);
};


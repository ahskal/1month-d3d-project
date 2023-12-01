#pragma once

class Unit : public Actor
{
public:
	static Unit* Create(string name = "Unit");

	Unit();
	~Unit();

	float Hp, MaxHp;
	float Attack;
	float Defense;

	
	//�ѹ��� ����Ǵ� ����
	bool Once = true;

	float hitCooltime;
	bool hit;

	bool isAttack;

	void Update();
	void Render(shared_ptr<Shader> pShader = nullptr) override;
	virtual void	RenderDetail();

	void Damage(int damage);
	



};
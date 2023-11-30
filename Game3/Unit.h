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

	//한번만 실행되는 변수
	bool Once = true;

	void Update();
	void Render(shared_ptr<Shader> pShader = nullptr) override;
	virtual void	RenderDetail();
};
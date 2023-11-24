#pragma once
class Unit : public Actor
{
public:
	static void Create(string name = "Unit");
	static Unit* Create(Unit* src);
private:



public:
	Unit();
	~Unit();

	//가상함수
	virtual void Init() {};
	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void Render() {};
	virtual void PreRender() {};
};


#pragma once

class UI_Monster : public Billboard
{
public:
	static UI_Monster* Create(string name = "UI_Monster");
private:
	class Monster* monster;

public:
	UI_Monster();
	~UI_Monster();
	void Update() override;
	void Render(shared_ptr<Shader> pShader = nullptr) override;

	Monster* GetData() const { return monster; }
	void SetData(Monster* newData) { monster = newData; }
	void VisibleOFF() { visible = false; }
};


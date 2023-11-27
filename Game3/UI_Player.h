#pragma once
class UI_Player : public UI
{
public:
	static UI_Player* Create(string name = "UI_Player");
private:
	class Player* player;
public:
	UI_Player();
	~UI_Player();
	void Update() override;
	void Render(shared_ptr<Shader> pShader = nullptr) override;

	Player* GetData() const { return player; }
	void SetData(Player* newData) { player = newData; }
};


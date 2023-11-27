#pragma once
class UI_Player : public UI
{
public:
	static UI_Player* Create(string name = "UI_Player");
private:
	Status status;

public:
	UI_Player();
	~UI_Player();
	void SetStatus(Status status);
	void Update() override;
	void Render(shared_ptr<Shader> pShader = nullptr) override;
};


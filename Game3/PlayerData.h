#pragma once



class PlayerData
{
public:
	class Player* play;
	class UI_Player* UI;
	class PlayerObserver* pObserver;
	// �߰� ������ �ʵ��

	static PlayerData* Create(string name = "playerData");
	PlayerData();
	void Update();
	void Render();
	void DeferredRender(shared_ptr<Shader> pShader = nullptr);

};


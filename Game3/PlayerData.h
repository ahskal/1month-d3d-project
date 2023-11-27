#pragma once



class PlayerData
{
public:
	class Player* play;
	class UI_Player* UI;
	class PlayerObserver* pObserver;
	// 추가 데이터 필드들

	static PlayerData* Create(string name = "playerData");
	PlayerData();
	void Update();
	void Render();
	void DeferredRender(shared_ptr<Shader> pShader = nullptr);

};


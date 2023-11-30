#pragma once

class PlayerData
{
public:
	static PlayerData* Create(string name = "PlayerData");
private:
	class Player* actor;
public:
	class UI_Player* UI;
	class PlayerObserver* pObserver;
	// 추가 데이터 필드들

	PlayerData();

	//메인 카메라 설정
	void MainCamSet();

	void Update();
	void LateUpdate();

	void Hierarchy();

	//기본 UI 혹은 쉐이더를 받으면서 하는 렌더링
	void Render(shared_ptr<Shader> pShader = nullptr);

	// 디퍼드 렌더링을 위한 객체 렌더링
	void DeferredRender(shared_ptr<Shader> pShader = nullptr);




};


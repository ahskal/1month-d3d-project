#pragma once
class MonsterData {
public:
	class Monster* Mon;
	class UI_Monster* UI;
	class MonsterObserver* mObserver;
	// 추가 데이터 필드들

	MonsterData(Vector3 pos);
	void Update();
	void Render();
	void DeferredRender(shared_ptr<Shader> pShader = nullptr);
};

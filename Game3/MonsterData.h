#pragma once
class MonsterData {
public:
	class Monster* Mon;
	class UI_Monster* UI;
	class MonsterObserver* mObserver;
	// �߰� ������ �ʵ��

	MonsterData(Vector3 pos);
	void Update();
	void Render();
	void DeferredRender(shared_ptr<Shader> pShader = nullptr);
};

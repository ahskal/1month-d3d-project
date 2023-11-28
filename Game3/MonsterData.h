#pragma once
class MonsterData {
public:
	static MonsterData* Create(string name = "MonsterData");

	class Monster* Mon;
	class UI_Monster* UI;
	class MonsterObserver* mObserver;
	// �߰� ������ �ʵ��

	MonsterData();
	~MonsterData();

	void Update();
	void Render(shared_ptr<Shader> pShader = nullptr);
	void DeferredRender(shared_ptr<Shader> pShader = nullptr);
};

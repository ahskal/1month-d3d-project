#pragma once


class MonsterData {
public:
	class Monster* Mon;
	class UI_Monster* UI;
	class MonsterObserver* mObserver;
	// �߰� ������ �ʵ��

	MonsterData(Vector3 pos);

	void Update(); 

	void DeferredRender(shared_ptr<Shader> pShader = nullptr);

	void Render();
};

class MonsterManager : public Singleton<MonsterManager>
{
	vector<MonsterData*> MonVec;

public:
	MonsterManager();
	~MonsterManager();

	void CreateMonster(Vector3 pos);
	void Update();
	void LateUpdate();
	void Render(shared_ptr<Shader> pShader = nullptr);

	vector<MonsterData*> GetMonsterVector() { return MonVec; }
};


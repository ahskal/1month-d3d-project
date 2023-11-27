#pragma once

class MonsterManager : public Singleton<MonsterManager>
{
	vector<class MonsterData*> MonVec;

public:
	MonsterManager();
	~MonsterManager();

	void CreateMonster(Vector3 pos);
	void GetTargetPos(Vector3 pos);
	void Update();
	void LateUpdate();
	void Render(shared_ptr<Shader> pShader = nullptr);

	vector<class MonsterData*> GetMonsterVector() const { return MonVec; }
};


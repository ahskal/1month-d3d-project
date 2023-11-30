#include "stdafx.h"
#include "MonsterData.h"
#include "Monster.h"
#include "MonsterManager.h"

MonsterManager::MonsterManager()
{
	MonVec.resize(0);
}

MonsterManager::~MonsterManager()
{
}

void MonsterManager::CreateMonster(Vector3 pos)
{
	MonsterData* Mon = new MonsterData();
	Mon->Mon->SetSpawn(pos);
	MonVec.push_back(Mon);
}

void MonsterManager::GetTargetPos(Vector3 pos)
{
	for (auto it : MonVec) {
		it->Mon->GetTargerPos(pos);
	}
}

void MonsterManager::Update()
{
	for (auto it : MonVec) {
		it->Update();
	}
}

void MonsterManager::LateUpdate()
{
	//���� ���� �ݺ���
	MonVec.erase(
		std::remove_if(
			MonVec.begin(),
			MonVec.end(),
			[&](MonsterData* Md) {
				// ���� ü���� 0 ������ ��� �ش� ���͸� �����Ϸ��� true�� ��ȯ
				// �׷��� ���� ��� �����Ϸ��� false�� ��ȯ
				bool Remove = Md->Mon->Hp <= 0;
				if (Remove) {
					Md->Mon->SetState("��ü ����");
					delete Md;
				}
				return Remove;
			}
		),
		MonVec.end()// remove_if�� ���� �ڷ� �Ű��� ���ҵ��� ���� ����
				);
}

void MonsterManager::Render(shared_ptr<Shader> pShader)
{
	for (auto it : MonVec) {
		it->DeferredRender(pShader);
		it->Render();
	}
}


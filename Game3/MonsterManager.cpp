#include "stdafx.h"

#include "Monster.h"
#include "UI_Monster.h"
#include "MonsterObserver.h"
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
	MonsterData* Mon = new MonsterData(pos);
	MonVec.push_back(Mon);
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
				bool Remove = Md->mObserver->GetStatus().Hp <= 0;
				if (Remove) {
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

MonsterData::MonsterData(Vector3 pos)
{
	Mon = Monster::Create();
	Mon->LoadFile("Unit/Man2.xml");

	mObserver = new MonsterObserver(Mon);
	Mon->Attach(mObserver);

	UI = UI_Monster::Create();
	UI->LoadFile("UI/Hpbar.xml");

	Mon->SetSpawn(pos);
	UI->parent = Mon;

	Mon->UpdateObserver();
	UI->SetStatus(mObserver->GetStatus());
	// �߰� ������ �ʱ�ȭ
}

void MonsterData::Update()
{
	Mon->Update();
	UI->SetStatus(mObserver->GetStatus());
	UI->Update();
}

void MonsterData::DeferredRender(shared_ptr<Shader> pShader)
{
	Mon->Render(pShader);
}

void MonsterData::Render()
{
	UI->Render();
}

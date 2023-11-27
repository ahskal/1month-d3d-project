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
	//몬스터 삭제 반복문
	MonVec.erase(
		std::remove_if(
			MonVec.begin(),
			MonVec.end(),
			[&](MonsterData* Md) {
				// 현재 체력이 0 이하인 경우 해당 몬스터를 제거하려면 true를 반환
				// 그렇지 않은 경우 유지하려면 false를 반환
				bool Remove = Md->mObserver->GetStatus().Hp <= 0;
				if (Remove) {
					delete Md;
				}
				return Remove;
			}
		),
		MonVec.end()// remove_if를 통해 뒤로 옮겨진 원소들의 시작 지점
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
	// 추가 데이터 초기화
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

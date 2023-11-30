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
	//몬스터 삭제 반복문
	MonVec.erase(
		std::remove_if(
			MonVec.begin(),
			MonVec.end(),
			[&](MonsterData* Md) {
				// 현재 체력이 0 이하인 경우 해당 몬스터를 제거하려면 true를 반환
				// 그렇지 않은 경우 유지하려면 false를 반환
				bool Remove = Md->Mon->Hp <= 0;
				if (Remove) {
					Md->Mon->SetState("객체 삭제");
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


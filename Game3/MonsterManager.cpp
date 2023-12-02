#include "stdafx.h"
#include "MonsterData.h"
#include "Monster.h"
#include "UI_Monster.h"
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
	for (auto it : MonVec) {
		if (it->Mon->Hp <= 0 && not it->Mon->isDead) {
			it->Mon->SetState(Mon::State::DEADSTART);
			it->Mon->isDead = true;
			it->Mon->SlashOFF();
			it->UI->VisibleOFF();
			switch (RANDOM->Int(1, 4)) {
			case 1: it->Mon->anim->ChangeAnimation(AnimationState::ONCE_LAST, Mon::Ani_Dead_01, 0.1f); break;
			case 2: it->Mon->anim->ChangeAnimation(AnimationState::ONCE_LAST, Mon::Ani_Dead_02, 0.1f); break;
			case 3: it->Mon->anim->ChangeAnimation(AnimationState::ONCE_LAST, Mon::Ani_Dead_03, 0.1f); break;
			case 4: it->Mon->anim->ChangeAnimation(AnimationState::ONCE_LAST, Mon::Ani_Dead_04, 0.1f); break;
			}
		}
	}

	//���� ���� �ݺ���
	MonVec.erase(
		std::remove_if(
			MonVec.begin(),
			MonVec.end(),
			[&](MonsterData* Md) {
				// ���� ü���� 0 ������ ��� �ش� ���͸� �����Ϸ��� true�� ��ȯ
				// �׷��� ���� ��� �����Ϸ��� false�� ��ȯ
				bool Remove = Md->Mon->GetState() == Mon::State::DEADEND;
				if (Remove) {
					int val = RANDOM->Int(1, 5);
					auto M = ITEM->CreateItem("Gold", new MoneyItemFactory);
					for (int i = 0; i < val; i++) {
						Vector3 dropPos = Md->Mon->GetWorldPos() + Vector3(RANDOM->Int(-3, 0), 0, RANDOM->Int(-3, 0));
						FIELD->AddItem(M, dropPos);
					}

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
		it->Render(pShader);
	}
}

void MonsterManager::Hierarchy()
{
	for (auto it : MonVec) {
		it->Hierarchy();
	}
}


#include "stdafx.h"
#include "Monster.h"
#include "UI_Monster.h"
#include "MonsterObserver.h"
#include "MonsterData.h"

MonsterData* MonsterData::Create(string name)
{
	MonsterData* MD = new MonsterData();
	MD->Mon->name = name;
	return MD;
}

MonsterData::MonsterData()
{
	Mon = Monster::Create();
	Mon->LoadFile("Unit/Man2.xml");

	mObserver = new MonsterObserver(Mon);
	Mon->Attach(mObserver);

	UI = UI_Monster::Create();
	UI->LoadFile("UI/Hpbar.xml");
	UI->parent = Mon;
	UI->SetData(Mon);

	// 추가 데이터 초기화
}

void MonsterData::Update()
{
	Mon->Update();
	UI->Update();
}

void MonsterData::Render(shared_ptr<Shader> pShader)
{
	UI->Render(pShader);
}

void MonsterData::DeferredRender(shared_ptr<Shader> pShader)
{
	Mon->Render(pShader);
}

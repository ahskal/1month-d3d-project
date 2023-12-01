#include "stdafx.h"
#include "Monster.h"
#include "UI_Monster.h"
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

	UI = UI_Monster::Create();
	UI->LoadFile("UI/Hpbar.xml");
	UI->parent = Mon;
	UI->SetData(Mon);
}

MonsterData::~MonsterData()
{
	delete UI;
	delete Mon;
}

void MonsterData::Update()
{
	Mon->Update();
	if (Mon->GetState() != Mon::State::IDLE) {
		UI->visible = true;
	}
	else {
		UI->visible = false;
	}
	UI->Update();
}

void MonsterData::Render(shared_ptr<Shader> pShader)
{
	Mon->Render(pShader);
	UI->Render();
}

void MonsterData::Hierarchy()
{
	Mon->RenderHierarchy();
	UI->RenderHierarchy();
}

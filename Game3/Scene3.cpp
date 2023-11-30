#include "stdafx.h"

#include "PlayerData.h"
#include "Player.h"
#include "UI_Player.h"
#include "PlayerObserver.h"

#include "Monster.h"
#include "UI_Monster.h"
#include "MonsterObserver.h"
#include "MonsterData.h"


#include "Scene3.h"


extern bool DEBUG_MODE;

Scene3::Scene3()
{
	grid = Grid::Create();

	cam1 = Camera::Create();
	cam1->LoadFile("Cam2.xml");

	deferred = new Deferred;
	post = UI::Create();
	post->LoadFile("Deferred.xml");

	player = PlayerData::Create();
	player->MainCamSet();
	ResizeScreen();

	MonMGR->CreateMonster(Vector3(RANDOM->Int(-10, 10), 0, RANDOM->Int(-10, 10)));
}

Scene3::~Scene3()
{
}

void Scene3::Init()
{

}

void Scene3::Release() {}

void Scene3::Update()
{
	ImGui::Text("FPS : %d", TIMER->GetFramePerSecond());

	if (DEBUG_MODE) {
		ImGui::Text("MonsterVector Size : %d ", MonMGR->GetMonsterVector().size());
		deferred->RenderDetail();
	}

	LIGHT->RenderDetail();

	ImGui::Begin("Hierarchy", nullptr);
	grid->RenderHierarchy();
	cam1->RenderHierarchy();
	player->Hierarchy();

	auto Monster = MonMGR->GetMonsterVector();
	for (auto Mvector : Monster) {
		Mvector->UI->RenderHierarchy();
	}

	ImGui::End();

	post->Update();
	grid->Update();

	player->Update();
	Camera::main->Update();
	MonMGR->Update();
	MonMGR->GetTargetPos(player->pObserver->GetData()->GetWorldPos());

	//Camera::main->ControlMainCam();

	if (INPUT->KeyDown('T')) {
		MonMGR->CreateMonster(Vector3(RANDOM->Int(-10, 10), 0, RANDOM->Int(-10, 10)));
	}

}

void Scene3::LateUpdate()
{

	Vector3 pPos = player->pObserver->GetPos();
	
	auto* col = player->pObserver->GetSword();
	
	auto Monster = MonMGR->GetMonsterVector();
	for (auto Mvector : Monster) {
		auto Mon = Mvector->Mon;
		if (Mon->Intersect(col)) {
			if (player->pObserver->GetData()->isHit == false) {
				player->pObserver->GetData()->isHit = true;
				Mon->Hp -= 50;
				Mon->SetState("공격받음");
			}
		}
	};
	player->LateUpdate();
	MonMGR->LateUpdate();
}

void Scene3::PreRender()
{
	LIGHT->Set();
	Camera::main->Set();

	deferred->SetTarget();

	player->DeferredRender(RESOURCE->shaders.Load("4.Cube_Deferred.hlsl"));
	auto Monster = MonMGR->GetMonsterVector();
	for (auto Mvector : Monster) {
		Mvector->DeferredRender(RESOURCE->shaders.Load("4.Cube_Deferred.hlsl"));
		
		Mvector->Render();
	}
	player->EffectRender();
}

void Scene3::Render()
{
	deferred->Render();
	grid->Render();

	player->Render();


	auto Monster = MonMGR->GetMonsterVector();
	for (auto Mvector : Monster) {
	}
}

void Scene3::ResizeScreen()
{
	Camera::main->viewport.x = 0.0f;
	Camera::main->viewport.y = 0.0f;
	Camera::main->viewport.width = App.GetWidth();
	Camera::main->viewport.height = App.GetHeight();
	Camera::main->width = App.GetWidth();
	Camera::main->height = App.GetHeight();
}

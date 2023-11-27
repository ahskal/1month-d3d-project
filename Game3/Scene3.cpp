#include "stdafx.h"
#include "Subject.h"

#include "Player.h"
#include "UI_Player.h"
#include "PlayerObserver.h"

#include "Monster.h"
#include "UI_Monster.h"
#include "MonsterObserver.h"


#include "Scene3.h"


extern bool DEBUG_MODE;

Scene3::Scene3()
{
	grid = Grid::Create();
	cam1 = Camera::Create();
	cam1->LoadFile("Cam2.xml");
	Camera::main = cam1;
	deferred = new Deferred;
	post = UI::Create();
	post->LoadFile("Deferred.xml");

	player = Player::Create();
	pObserver = new PlayerObserver(player);
	player->Attach(pObserver);
	pUI = UI_Player::Create();
	pUI->LoadFile("UI/Main_Layout_1.xml");
	Camera::main = static_cast<Camera*>(player->Find("Camera"));

	ResizeScreen();
}

Scene3::~Scene3()
{
}

void Scene3::Init()
{
	player->SetSpawn(Vector3(0, 0, 0));
	MonMGR->CreateMonster(Vector3(RANDOM->Int(-10, 10), 0, RANDOM->Int(-10, 10)));
}

void Scene3::Release()
{
}

void Scene3::Update()
{
	ImGui::Text("FPS : %d", TIMER->GetFramePerSecond());
	ImGui::Text("MonsterVector Size : %d ", MonMGR->GetMonsterVector().size());
	//deferred->RenderDetail();

	LIGHT->RenderDetail();

	ImGui::Begin("Hierarchy", nullptr);
	grid->RenderHierarchy();
	cam1->RenderHierarchy();

	player->RenderHierarchy();
	pUI->RenderHierarchy();


	ImGui::End();

	post->Update();
	grid->Update();
	Camera::main->Update();
	//Camera::main->ControlMainCam();

	player->Update();
	pUI->SetStatus(pObserver->GetStatus());
	pUI->Update();

	if (INPUT->KeyDown('T')) {
		MonMGR->CreateMonster(Vector3(RANDOM->Int(-10, 10), 0, RANDOM->Int(-10, 10)));
	}


	MonMGR->Update();
}

void Scene3::LateUpdate()
{
	player->LateUpdate();

	auto pPos = player->GetWorldPos();

	auto col = player->Find("sword");

	auto Monster = MonMGR->GetMonsterVector();
	for (auto Mvector : Monster) {
		auto Mon = Mvector->Mon;
		if (Mon->Intersect(col)) {
			if (player->isHit == false) {
				player->isHit = true;
				Mon->Hp -= 50;
				Mon->SetState("공격받음");
			}
		}
		Vector3 pos = Mon->GetWorldPos() - pPos;
		ImGui::Text("length : %f", pos.Length());
	};
	MonMGR->LateUpdate();

}

void Scene3::PreRender()
{
	LIGHT->Set();
	Camera::main->Set();

	deferred->SetTarget();
	player->Render(RESOURCE->shaders.Load("4.Cube_Deferred.hlsl"));

	auto Monster = MonMGR->GetMonsterVector();
	for (auto Mvector : Monster) {
		Mvector->DeferredRender(RESOURCE->shaders.Load("4.Cube_Deferred.hlsl"));
	}
}

void Scene3::Render()
{
	deferred->Render();
	grid->Render();

	pUI->Render();

	player->LateRender();
	auto Monster = MonMGR->GetMonsterVector();
	for (auto Mvector : Monster) {
		Mvector->Render();
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

#include "stdafx.h"

#include "PlayerData.h"
#include "Player.h"
#include "UI_Player.h"
#include "PlayerObserver.h"

#include "Monster.h"
#include "UI_Monster.h"
#include "MonsterData.h"

#include "MapGenerator.h"

#include "Scene2.h"

extern bool DEBUG_MODE;

Scene2::Scene2()
{

	cam1 = Camera::Create();
	cam1->LoadFile("Cam2.xml");

	player = PlayerData::Create();
	player->MainCamSet();
	ResizeScreen();

	skybox = Sky::Create();
	skybox->LoadFile("Sky1.xml");
	skybox2 = Sky::Create();
	skybox2->LoadFile("Sky2.xml");

	Tile = Actor::Create();
	Tile->name = "Tile";
	int rows = 20;
	int cols = 20;
	int floors = 1;

	// Create an instance of MapGenerator
	mapGen = new MapGenerator(rows, cols, floors);

	deferred = new Deferred;
	post = UI::Create();
	post->LoadFile("Deferred.xml");

	LIGHT->dirLight.color = Color(0, 0, 0);
}

Scene2::~Scene2()
{
}

void Scene2::Init()
{
	//맵 초기값 설정 및 생성
	mapGen->generateInitialMap();
	mapGen->checkConnectivity();
	//mapGen->coutTile();
	mapGen->InstanceTile(Tile);
	mapGen->finalizeMap(Tile);
	mapGen->WallCreateMap(Tile);

	int count;
	for (int k = 0; k < mapGen->floors; ++k) {
		int x;
		int y;
		do
		{
			x = RANDOM->Int(0, mapGen->rows - 1);
			y = RANDOM->Int(0, mapGen->cols - 1);
			count = 0;
			for (int i = max(0, x - 1); i <= min(mapGen->rows - 1, x); ++i) {
				for (int j = max(0, y - 1); j <= min(mapGen->cols - 1, y); ++j) {
					// 현재 좌표 (i, j, k)의 타일을 검사
					// 수정된 부분: 중심 타일을 기준으로 주변 2x2 영역만 검사
					if (mapGen->Tiles[i][j][k] == 1) {
						count++;
					}
				}
			}
		} while (count != 4);
		Actor* temp = Actor::Create();
		temp->LoadFile("box.xml");
		temp->name = "box" + to_string(k);
		temp->SetLocalPosY(0);
		Tile->Find(to_string(x) + "x" + to_string(y) + "x" + to_string(k))->AddChild(temp);
	}


	int x;
	int y;
	do
	{
		x = RANDOM->Int(0, mapGen->rows - 1);
		y = RANDOM->Int(0, mapGen->cols - 1);
		count = 0;
		for (int i = max(0, x - 1); i <= min(mapGen->rows - 1, x); ++i) {
			for (int j = max(0, y - 1); j <= min(mapGen->cols - 1, y); ++j) {
				// 현재 좌표 (i, j, k)의 타일을 검사
				// 수정된 부분: 중심 타일을 기준으로 주변 2x2 영역만 검사
				if (mapGen->Tiles[i][j][0] == 1) {
					count++;
				}
			}
		}
	} while (count != 4);
	float mapSize = ((mapGen->rows * mapGen->tileSize) / 2);
	player->pObserver->GetData()->SetSpawn(Vector3(-mapSize + x * 5, 0, -mapSize + y * 5));



}

void Scene2::Release()
{
}

void Scene2::Update()
{
	//deferred->RenderDetail();

	static bool isOnece = true;
	if (isOnece) {

		isOnece = false;
	}

	// # DEBUG #
	LIGHT->RenderDetail();

	ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());

	// # DEBUG #


	ImGui::Begin("Hierarchy", nullptr);
	cam1->RenderHierarchy();
	Tile->RenderHierarchy();

	player->Hierarchy();

	ImGui::End();

	if (DEBUG_MODE) {
		Camera::main->ControlMainCam();
		Camera::main = cam1;
		ResizeScreen();
		for (auto it : mapGen->WallActorList) {
			it->visible = true;
		}
	}
	else {
		player->MainCamSet();
		ResizeScreen();
	}

	if (INPUT->KeyDown(VK_F5)) {
		Tile->ReleaseMember();
		Init();
	}

	if (INPUT->KeyDown('T')) {
		int count;
		int x;
		int y;
		do
		{
			x = RANDOM->Int(0, mapGen->rows - 1);
			y = RANDOM->Int(0, mapGen->cols - 1);
			count = 0;
			for (int i = max(0, x - 1); i <= min(mapGen->rows - 1, x); ++i) {
				for (int j = max(0, y - 1); j <= min(mapGen->cols - 1, y); ++j) {
					// 현재 좌표 (i, j, k)의 타일을 검사
					// 수정된 부분: 중심 타일을 기준으로 주변 2x2 영역만 검사
					if (mapGen->Tiles[i][j][0] == 1) {
						count++;
					}
				}
			}
		} while (count != 1);
		float mapSize = ((mapGen->rows * mapGen->tileSize) / 2);
		MonMGR->CreateMonster(Vector3(-mapSize + x * 5, 0, -mapSize + y * 5));
	}

	Camera::main->Update();

	skybox->Update();
	skybox2->Update();
	Tile->Update();
	post->Update();

	player->Update();

	//MonMGR->Update();
	MonMGR->GetTargetPos(player->pObserver->GetData()->GetWorldPos());
}

void Scene2::LateUpdate()
{
	Vector3 playerPos = player->pObserver->GetPos();

	static bool isOnece = false;
	if (isOnece) {
		if (!mapGen->GetTileState(playerPos)) {
			player->pObserver->GetData()->GoBack();
		}
		auto Monster = MonMGR->GetMonsterVector();
		for (auto Mvector : Monster) {
			Vector3 monsterPos = Mvector->Mon->GetWorldPos();
			if (!mapGen->GetTileState(monsterPos)) {
				Mvector->Mon->GoBack();
			}
		}
	}
	isOnece = true;

	auto Monster = MonMGR->GetMonsterVector();
	for (auto Mvector : Monster) {
		Vector3 monsterPos = Mvector->Mon->GetWorldPos();
		Vector3 mDir = Mvector->Mon->GetForward();
		for (auto Wcoll : mapGen->WallActorList) {
			float ForwardAngle = Wcoll->GetForward().Dot(mDir);
			float RightAngle = Wcoll->GetRight().Dot(mDir);
			if (fabs(ForwardAngle) < fabs(RightAngle)) {

				Mvector->Mon->MoveWorldPos(Wcoll->GetForward() * DELTA * 10);
			}
			else {
				Mvector->Mon->MoveWorldPos(Wcoll->GetRight() * DELTA * 10);
			}
		}


	}
	Vector3 pDir = player->pObserver->GetData()->GetForward();

	for (auto Wcoll : mapGen->WallActorList) {

		if (Wcoll->Intersect(player->pObserver->GetData())) {
			float ForwardAngle = Wcoll->GetForward().Dot(pDir);
			float RightAngle = Wcoll->GetRight().Dot(pDir);
			if (fabs(ForwardAngle) < fabs(RightAngle)) {
				player->pObserver->GetData()->MoveWorldPos(-Wcoll->GetForward() * DELTA * 6);
			}
			else {
				player->pObserver->GetData()->MoveWorldPos(-Wcoll->GetRight() * DELTA * 6);
			}
		}
	}


	MonMGR->LateUpdate();
}

void Scene2::PreRender()
{
	Camera::main->Set();
	LIGHT->Set();

	deferred->SetTarget();
	Tile->Render();

	auto Monster = MonMGR->GetMonsterVector();
	for (auto Mvector : Monster) {
		Mvector->DeferredRender(RESOURCE->shaders.Load("4.Cube_Deferred.hlsl"));
		Mvector->Render(RESOURCE->shaders.Load("7.Billboard_Deferred.hlsl"));
	}
	player->DeferredRender(RESOURCE->shaders.Load("4.Cube_Deferred.hlsl"));
}

void Scene2::Render()
{
	auto playerCam = player->pObserver->GetData()->Find("Camera");
	for (auto it : mapGen->WallActorList) {
		if (it->Intersect(playerCam)) {
			it->visible = false;
		}
		else {
			it->visible = true;;
		}
	}

	skybox->Render();
	skybox2->Render();

	deferred->Render();
	player->Render();

}

void Scene2::ResizeScreen()
{
	Camera::main->viewport.x = 0.0f;
	Camera::main->viewport.y = 0.0f;
	Camera::main->viewport.width = App.GetWidth();
	Camera::main->viewport.height = App.GetHeight();
	Camera::main->width = App.GetWidth();
	Camera::main->height = App.GetHeight();
}

#include "stdafx.h"

#include "PlayerData.h"
#include "Player.h"
#include "UI_Player.h"
#include "PlayerObserver.h"
#include "Inventory.h"

#include "Monster.h"
#include "UI_Monster.h"
#include "MonsterData.h"

#include "MapGenerator.h"

#include "Scene2.h"
#include "Scene3.h"

extern bool DEBUG_MODE;
extern bool NONE_SCENE;
extern bool TEXT_LOG;
extern bool FREE_CAM;

Scene2::Scene2()
{
	cam1 = Camera::Create();
	cam1->LoadFile("Cam2.xml");
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

	PLAYER->MainCamSet();
	ResizeScreen();

	deferred = new Deferred;

	act = Actor::Create();

}

Scene2::~Scene2()
{
	skybox2->Release();
	skybox->Release();
	cam1->Release();
	Tile->Release();
	delete mapGen;
	MONSTER->vectorMemberClear();
}

void Scene2::Init()
{
	//맵 초기값 설정 및 생성
	mapGen->initializeMap();
	//mapGen->updateMapTiles();
	mapGen->validateMapConnectivity();
	//mapGen->printTileInfo();
	mapGen->instantiateTile(Tile);
	mapGen->finalizeMapConfiguration(Tile);
	mapGen->createWallMap(Tile);
	mapGen->createLighting(Tile);
	mapGen->createPotal(Tile);
	
	Vector3 pos = mapGen->TileRandomPos();
	PLAYER->actor->SetWorldPos(pos);


}

void Scene2::Release()
{
}

void Scene2::Update()
{
	// # DEBUG #
	LIGHT->RenderDetail();
	ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());
	if (DEBUG_MODE) {
		deferred->RenderDetail();
	}
	// # DEBUG #

	ImGui::Begin("Hierarchy", nullptr);
	cam1->RenderHierarchy();
	Tile->RenderHierarchy();
	PLAYER->Hierarchy();
	act->RenderHierarchy();
	//MonMGR->Hierarchy();
	//FIELD->Hierarchy();

	ImGui::End();

	if (FREE_CAM) {
		Camera::main->ControlMainCam();
		Camera::main = cam1;
		ResizeScreen();
		for (auto it : mapGen->WallActorList) {
			it->visible = true;
		}
	}
	else {
		PLAYER->MainCamSet();
		ResizeScreen();
	}

	// map reset;


	if (INPUT->KeyDown('T')) {
		Vector3 pos = mapGen->TileRandomPos();
		MONSTER->CreateMonster(pos);
	}
	if (INPUT->KeyDown('G')) {
		PLAYER->inventory->OpenList();
	}

	Camera::main->Update();

	skybox->Update();
	skybox2->Update();
	Tile->Update();

	PLAYER->Update();

	MONSTER->Update();
	MONSTER->GetTargetPos(PLAYER->pObserver->GetData()->GetWorldPos());
	FIELD->Update();
	DAMAGEFONT->Update();
	act->Update();
}

void Scene2::LateUpdate()
{
	//static bool isOnece = false;
	//if (INPUT->KeyDown(VK_F5)) {
	//	Tile->ReleaseMember();

	//	isOnece = false;
	//}
	//if (INPUT->KeyDown(VK_F4)) {
	//	Init();
	//}
	{
		const float SLIDING_SPEED = DELTA * 10;
		Vector3 pDir = PLAYER->actor->GetForward();
		for (auto Wcoll : mapGen->WallActorList) {
			if (INPUT->KeyPress('S')) {
				Vector3 pDir = -PLAYER->actor->GetForward();
			}
			if (Wcoll->Intersect(PLAYER->pObserver->GetData())) {
				float ForwardAngle = Wcoll->GetForward().Dot(pDir);
				float RightAngle = Wcoll->GetRight().Dot(pDir);
				Vector3 slideDirection;
				if (ForwardAngle > 0 && RightAngle > 0)
				{
					slideDirection -= Wcoll->GetForward();
					slideDirection -= Wcoll->GetRight();
				}
				else if (ForwardAngle > 0 && RightAngle < 0)
				{
					slideDirection -= Wcoll->GetForward();
					slideDirection += Wcoll->GetRight();
				}
				else if (ForwardAngle < 0 && RightAngle > 0)
				{
					slideDirection += Wcoll->GetForward();
					slideDirection -= Wcoll->GetRight();
				}
				else if (ForwardAngle < 0 && RightAngle < 0)
				{
					slideDirection += Wcoll->GetForward();
					slideDirection += Wcoll->GetRight();
				}
				// Normalize the slide direction
				slideDirection.Normalize();
				PLAYER->pObserver->GetData()->MoveWorldPos(slideDirection * SLIDING_SPEED);
			}
		}
		//vector<MonsterData*> Monster = MonMGR->GetMonsterVector();
		//for (MonsterData* Mvector : Monster) {
		//	Vector3 monsterPos = Mvector->Mon->GetWorldPos();
		//	Vector3 mDir = Mvector->Mon->GetForward();
		//	for (auto Wcoll : mapGen->WallActorList) {
		//		float ForwardAngle = Wcoll->GetForward().Dot(mDir);
		//		float RightAngle = Wcoll->GetRight().Dot(mDir);
		//		Vector3 slideDirection;
		//		if (ForwardAngle > 0 && RightAngle > 0)
		//		{
		//			slideDirection -= Wcoll->GetForward();
		//			slideDirection -= Wcoll->GetRight();
		//		}
		//		else if (ForwardAngle > 0 && RightAngle < 0)
		//		{
		//			slideDirection -= Wcoll->GetForward();
		//			slideDirection += Wcoll->GetRight();
		//		}
		//		else if (ForwardAngle < 0 && RightAngle > 0)
		//		{
		//			slideDirection += Wcoll->GetForward();
		//			slideDirection -= Wcoll->GetRight();
		//		}
		//		else if (ForwardAngle < 0 && RightAngle < 0)
		//		{
		//			slideDirection += Wcoll->GetForward();
		//			slideDirection += Wcoll->GetRight();
		//		}
		//		slideDirection.Normalize();
		//		Mvector->Mon->MoveWorldPos(slideDirection * 1);
		//	}
		//}

		//카메라 벽 충돌
		Camera* playerCam = static_cast<Camera*>(PLAYER->pObserver->GetData()->Find("Camera"));
		for (Actor* it : mapGen->WallActorList) {
			if (it->Intersect(playerCam)) {
				it->visible = false;
			}
			else {
				it->visible = true;;
			}
		}

	}

	// 포탈 충돌
	auto potal = Tile->Find("IcospherePotal");
	if (PLAYER->actor->Intersect(potal) or INPUT->KeyDown('0')) {
		PLAYER->actor->SetWorldPos(Vector3());
		SCENE->ChangeScene("SC3");
		SCENE->DeleteScene("SC2");
			return;
	}

	Vector3 pPos = PLAYER->actor->GetWorldPos();
	GameObject* PlayerSword = PLAYER->actor->Find("sword");
	vector<MonsterData*> Monster = MONSTER->GetMonsterVector();
	for (MonsterData* Mvector : Monster) {
		GameObject* MonsterSword = Mvector->Mon->Find("sword");

		auto Mon = Mvector->Mon;
		if (Mon->Intersect(PlayerSword) && PLAYER->actor->isAttack) {
			Mon->Damage(200);
		}
		else if (PLAYER->actor->Intersect(MonsterSword) && Mon->isAttack) {
			PLAYER->actor->Damage(5);
		}
	};

	for (auto it = FIELD->items.begin(); it != FIELD->items.end();) {
		if (PLAYER->actor->Intersect((*it)->actor)) {
			PLAYER->inventory->AddItem((*it));
			it = FIELD->ItemList().erase(it); // 현재 아이템 삭제 후 다음 아이템을 가리키도록 반복자를 업데이트
		}
		else {
			++it; // 아이템이 삭제되지 않았으면 다음 아이템으로 이동
		}
	}

	MONSTER->LateUpdate();


}

void Scene2::PreRender()
{
	Camera::main->Set();
	LIGHT->Set();

	deferred->SetTarget();
	Tile->Render();

	vector<MonsterData*> Monster = MONSTER->GetMonsterVector();
	for (MonsterData* Mvector : Monster) {
		Mvector->Render(RESOURCE->shaders.Load("4.Cube_Deferred.hlsl"));
	}
	PLAYER->DeferredRender(RESOURCE->shaders.Load("4.Cube_Deferred.hlsl"));
	FIELD->Render();
	DAMAGEFONT->Render(RESOURCE->shaders.Load("7.Billboard_Deferred.hlsl"));
	act->Render();

}

void Scene2::Render()
{
	//skybox->Render();
	//skybox2->Render();

	deferred->Render();
	PLAYER->Render();
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



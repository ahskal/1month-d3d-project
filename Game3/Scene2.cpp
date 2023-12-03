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

	player = PlayerData::Create();
	player->MainCamSet();
	ResizeScreen();

	deferred = new Deferred;
}

Scene2::~Scene2()
{
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

	//int count;
	//for (int k = 0; k < mapGen->floors; ++k) {
	//	int x;
	//	int y;
	//	do
	//	{
	//		x = RANDOM->Int(0, mapGen->rows - 1);
	//		y = RANDOM->Int(0, mapGen->cols - 1);
	//		count = 0;
	//		for (int i = max(0, x - 1); i <= min(mapGen->rows - 1, x); ++i) {
	//			for (int j = max(0, y - 1); j <= min(mapGen->cols - 1, y); ++j) {
	//				// 현재 좌표 (i, j, k)의 타일을 검사
	//				// 수정된 부분: 중심 타일을 기준으로 주변 2x2 영역만 검사
	//				if (mapGen->Tiles[i][j][k] == 1) {
	//					count++;
	//				}
	//			}
	//		}
	//	} while (count != 4);
	//	Actor* temp = Actor::Create();
	//	temp->LoadFile("box.xml");
	//	temp->name = "box" + to_string(k);
	//	temp->SetLocalPosY(0);
	//	Tile->Find(to_string(x) + "x" + to_string(y) + "x" + to_string(k))->AddChild(temp);
	//}

	int x = RANDOM->Int(2, 17);
	int y = RANDOM->Int(2, 17);
	if (mapGen->GetRandomPos(Int2(x, y))) {
		int tileWidth = mapGen->tileSize * mapGen->rows;
		int tileheight = mapGen->tileSize * mapGen->cols;
		int halfTileWidth = mapGen->tileSize * mapGen->rows / 2;

		Vector3 pos = Vector3(x * mapGen->tileSize - halfTileWidth / 2 + 2.5f, 0, y * mapGen->tileSize - halfTileWidth / 2 + 2.5f);
		player->actor->SetWorldPos(pos);
	}
	else {
		player->actor->SetWorldPos(Vector3());
	}
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
	player->Hierarchy();

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
		player->MainCamSet();
		ResizeScreen();
	}

	// map reset;


	if (INPUT->KeyDown('T')) {
		//int count;
		//int x;
		//int y;
		//do
		//{
		//	x = RANDOM->Int(0, mapGen->rows - 1);
		//	y = RANDOM->Int(0, mapGen->cols - 1);
		//	count = 0;
		//	for (int i = max(0, x - 1); i <= min(mapGen->rows - 1, x); ++i) {
		//		for (int j = max(0, y - 1); j <= min(mapGen->cols - 1, y); ++j) {
		//			// 현재 좌표 (i, j, k)의 타일을 검사
		//			// 수정된 부분: 중심 타일을 기준으로 주변 2x2 영역만 검사
		//			if (mapGen->Tiles[i][j][0] == 1) {
		//				count++;
		//			}
		//		}
		//	}
		//} while (count != 1);
		//float mapSize = ((mapGen->rows * mapGen->tileSize) / 2);
		//MonMGR->CreateMonster(Vector3(-mapSize + x * 5, 0, -mapSize + y * 5));
		MonMGR->CreateMonster(Vector3(10, 0, 10));
	}
	if (INPUT->KeyDown('G')) {
		player->inventory->OpenList();
	}

	Camera::main->Update();

	skybox->Update();
	skybox2->Update();
	Tile->Update();

	player->Update();

	MonMGR->Update();
	MonMGR->GetTargetPos(player->pObserver->GetData()->GetWorldPos());
	FIELD->Update();
	DAMAGEFONT->Update();

}

void Scene2::LateUpdate()
{
	static bool isOnece = false;
	if (INPUT->KeyDown(VK_F5)) {
		Tile->ReleaseMember();
		
		isOnece = false;
	}
	if (INPUT->KeyDown(VK_F4)) {
		Init();
	}
	// 벽 충돌 ( 처음 틱에는 한번 건너뛴다 )
	//Vector3 playerPos = player->pObserver->GetPos();	
	//	if (!mapGen->GetTileState(playerPos)) {
	//		player->pObserver->GetData()->GoBack();
	//	}
	//	vector<MonsterData*> Monster = MonMGR->GetMonsterVector();
	//	for (MonsterData* Mvector : Monster) {
	//		Vector3 monsterPos = Mvector->Mon->GetWorldPos();
	//		if (!mapGen->GetTileState(monsterPos)) {
	//			Mvector->Mon->GoBack();
	//		}
	//	}
	//}
	if (isOnece) 
	{
		const float SLIDING_SPEED = DELTA * 10;
		Vector3 pDir = player->actor->GetForward();
		for (auto Wcoll : mapGen->WallActorList) {
			if (Wcoll->Intersect(player->pObserver->GetData())) {
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
				player->pObserver->GetData()->MoveWorldPos(slideDirection * SLIDING_SPEED);
			}
		}
		vector<MonsterData*> Monster = MonMGR->GetMonsterVector();
		for (MonsterData* Mvector : Monster) {
			Vector3 monsterPos = Mvector->Mon->GetWorldPos();
			Vector3 mDir = Mvector->Mon->GetForward();
			for (auto Wcoll : mapGen->WallActorList) {
				float ForwardAngle = Wcoll->GetForward().Dot(mDir);
				float RightAngle = Wcoll->GetRight().Dot(mDir);
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
				slideDirection.Normalize();
				Mvector->Mon->MoveWorldPos(slideDirection * SLIDING_SPEED);
			}
		}

		//카메라 벽 충돌
		Camera* playerCam = static_cast<Camera*>(player->pObserver->GetData()->Find("Camera"));
		for (Actor* it : mapGen->WallActorList) {
			if (it->Intersect(playerCam)) {
				it->visible = false;
			}
			else {
				it->visible = true;;
			}
		}

	}




	Vector3 pPos = player->actor->GetWorldPos();
	GameObject* PlayerSword = player->actor->Find("sword");
	vector<MonsterData*> Monster = MonMGR->GetMonsterVector();
	for (MonsterData* Mvector : Monster) {
		GameObject* MonsterSword = Mvector->Mon->Find("sword");

		auto Mon = Mvector->Mon;
		if (Mon->Intersect(PlayerSword) && player->actor->isAttack) {
			Mon->Damage(200);
		}
		else if (player->actor->Intersect(MonsterSword) && Mon->isAttack) {
			player->actor->Damage(5);
		}
	};

	for (auto it = FIELD->items.begin(); it != FIELD->items.end();) {
		if (player->actor->Intersect((*it)->actor)) {
			player->inventory->AddItem((*it));
			it = FIELD->ItemList().erase(it); // 현재 아이템 삭제 후 다음 아이템을 가리키도록 반복자를 업데이트
		}
		else {
			++it; // 아이템이 삭제되지 않았으면 다음 아이템으로 이동
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

	vector<MonsterData*> Monster = MonMGR->GetMonsterVector();
	for (MonsterData* Mvector : Monster) {
		Mvector->Render(RESOURCE->shaders.Load("4.Cube_Deferred.hlsl"));
	}
	player->DeferredRender(RESOURCE->shaders.Load("4.Cube_Deferred.hlsl"));
	FIELD->Render();
	DAMAGEFONT->Render(RESOURCE->shaders.Load("7.Billboard_Deferred.hlsl"));

}

void Scene2::Render()
{
	//skybox->Render();
	//skybox2->Render();

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



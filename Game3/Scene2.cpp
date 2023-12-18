#include "stdafx.h"

#include "PlayerObserver.h"
#include "PlayerObserver.h"
#include "Inventory.h"

#include "Monster.h"
#include "UI_Monster.h"
#include "MonsterData.h"

#include "MapGenerator.h"

#include "Scene2.h"
#include "Scene3.h"

extern bool DEBUG_MODE;
extern bool TEXT_LOG;
extern bool FREE_CAM;
extern bool IS_PLAY;
extern bool SHOWCURSOR;
extern bool WIREFRAME;

extern int ThreadCount2;

Scene2::Scene2()
{
	cam1 = Camera::Create();
	cam1->LoadFile("Cam2.xml");
	ThreadCount2++;

	Tile = Actor::Create();
	Tile->name = "Tile";
	ThreadCount2++;

	int rows = 20;
	int cols = 20;
	int floors = 1;
	ThreadCount2++;

	// Create an instance of MapGenerator
	mapGen = new MapGenerator(rows, cols, floors);
	deferred = new Deferred;
	ThreadCount2++;

	act = Actor::Create();
	ThreadCount2++;

	//�� �ʱⰪ ���� �� ����
	mapGen->initializeMap();
	ThreadCount2++;
	//mapGen->updateMapTiles();
	mapGen->validateMapConnectivity();
	ThreadCount2++;
	//mapGen->printTileInfo();
	mapGen->instantiateTile(Tile);
	ThreadCount2++;
	mapGen->finalizeMapConfiguration(Tile);
	ThreadCount2++;
	mapGen->createWallMap(Tile);
	ThreadCount2++;
	mapGen->createLighting(Tile);
	ThreadCount2++;
	mapGen->createPotal(Tile);
	ThreadCount2++;
}

Scene2::~Scene2()
{
	MONSTER->vectorMemberClear();
	delete mapGen;
	act->Release();
	cam1->Release();
	Tile->Release();
	delete deferred;
}

void Scene2::Init()
{
	ThreadCount2 = 0;

	PLAYER->actor->SetWorldPos(mapGen->TileRandomPos());
	PLAYER->MainCamSet();

	for (int i = 0; i < 3; i++) {
		MONSTER->CreateMonster(mapGen->TileRandomPos());
	}
}

void Scene2::Release()
{
}

void Scene2::Update()
{
	// # DEBUG #
	deferred->RenderDetail();
	// # DEBUG #
	//LIGHT->RenderDetail();
	//ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());
	//if (DEBUG_MODE) {
	//	deferred->RenderDetail();
	//}
	//
	//
	/*ImGui::Begin("Hierarchy", nullptr);
	cam1->RenderHierarchy();
	Tile->RenderHierarchy();
	PLAYER->Hierarchy();
	MONSTER->Hierarchy();
	act->RenderHierarchy();


	ImGui::End();*/

	if (FREE_CAM) {
		Camera::main->ControlMainCam();
		Camera::main = cam1;
		ResizeScreen();
		for (auto it : mapGen->WallActorList) {
			it->visible = true;
		}
		SHOWCURSOR = true;
	}
	else {
		PLAYER->MainCamSet();
		ResizeScreen();
		SHOWCURSOR = false;
	}

	Camera::main->Update();

	Tile->Update();

	PLAYER->Update();
	if (IS_PLAY)
		PLAYER->UIUpdate();

	MONSTER->Update();

	FIELD->Update();
	DAMAGEFONT->Update();

	act->Update();

	if(MONSTER->GetMonsterVector().empty()) {
		for (int i = 0; i < 3; i++) {
			MONSTER->CreateMonster(mapGen->TileRandomPos());
		}
	}

}

void Scene2::LateUpdate()
{
	{
		const float SLIDING_SPEED = DELTA * 10;
		Vector3 pDir = PLAYER->actor->MoveDir;
		for (auto Wcoll : mapGen->WallActorList) {
			if (Wcoll->Intersect(PLAYER->actor)) {
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
		vector<MonsterData*> Monster = MONSTER->GetMonsterVector();
		for (MonsterData* Mvector : Monster) {
			Vector3 monsterPos = Mvector->Mon->GetWorldPos();
			Vector3 mDir = Mvector->Mon->moveDir;
			for (auto Wcoll : mapGen->WallActorList) {
				if (Wcoll->Intersect(Mvector->Mon)) {
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
					Mvector->Mon->MoveWorldPos(slideDirection * 1 * DELTA);
				}
			}
		}

		//ī�޶� �� �浹
		Camera* PLAYERCam = static_cast<Camera*>(PLAYER->pObserver->GetData()->Find("Camera"));
		for (Actor* it : mapGen->WallActorList) {
			if (it->Intersect(PLAYERCam)) {
				it->visible = false;
			}
			else {
				it->visible = true;;
			}
		}

	}

	// ��Ż �浹
	auto potal = Tile->Find("IcospherePotal");
	if (PLAYER->actor->Intersect(potal)) {
		PLAYER->actor->SetWorldPos(Vector3());
		SCENE->AddScene("SC3", new Scene3());
		SCENE->ChangeScene("SC3");
		return;
	}

	Vector3 pPos = PLAYER->actor->GetWorldPos();
	GameObject* PLAYERSword = PLAYER->actor->Find("sword");
	vector<MonsterData*> Monster = MONSTER->GetMonsterVector();
	for (MonsterData* Mvector : Monster) {
		GameObject* MonsterSword = Mvector->Mon->Find("sword");

		auto Mon = Mvector->Mon;
		if (Mon->Intersect(PLAYERSword) && PLAYER->actor->isAttack) {
			Mon->Damage(PLAYER->actor->Attack * 2.5);
		}
		else if (PLAYER->actor->Intersect(MonsterSword) && Mon->isAttack) {
			PLAYER->actor->Damage(Mon->Attack / 2);
		}
	};

	for (auto it = FIELD->items.begin(); it != FIELD->items.end();) {
		if (PLAYER->actor->Intersect((*it)->actor)) {
			PLAYER->inventory->AddItem((*it));
			it = FIELD->ItemList().erase(it); // ���� ������ ���� �� ���� �������� ����Ű���� �ݺ��ڸ� ������Ʈ
		}
		else {
			++it; // �������� �������� �ʾ����� ���� ���������� �̵�
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

	if (IS_PLAY)
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



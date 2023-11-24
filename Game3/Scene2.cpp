#include "stdafx.h"
#include "Player.h"
#include "Scene2.h"

bool DEBUGMODE = true;


Scene2::Scene2()
{
	grid = Grid::Create();

	cam1 = Camera::Create();
	cam1->LoadFile("Cam2.xml");

	player = Player::Create();

	Lightting = Light::Create();

	//player->scale *= 0.15;

	ResizeScreen();

	map = Terrain::Create("ground");
	//map->material->normalMap = RESOURCE->textures.Load("concrete_modular_1x1.png");
	map->material->LoadFile("TestMap1.mtl");
	map->CreateStructuredBuffer();
	//map->TreeCreateIntersect();
	map->garo = 100;
	map->MeshResizeUpdate();

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
	mapGen.set(rows, cols, floors);

	deferred = new Deferred;

	post = UI::Create();
	post->LoadFile("Deferred.xml");



	rain = Rain::Create();


}

Scene2::~Scene2()
{
}

void Scene2::Init()
{
	//맵 초기값 설정 및 생성
	mapGen.generateInitialMap();
	mapGen.checkConnectivity();
	//mapGen.coutTile();
	mapGen.InstanceTile(Tile);
	mapGen.finalizeMap(Tile);
	mapGen.WallCreateMap(Tile);

	int count;
	for (int k = 0; k < mapGen.floors; ++k) {
		int x;
		int y;
		do
		{
			x = RANDOM->Int(0, mapGen.rows - 1);
			y = RANDOM->Int(0, mapGen.cols - 1);
			count = 0;
			for (int i = max(0, x - 1); i <= min(mapGen.rows - 1, x); ++i) {
				for (int j = max(0, y - 1); j <= min(mapGen.cols - 1, y); ++j) {
					// 현재 좌표 (i, j, k)의 타일을 검사
					// 수정된 부분: 중심 타일을 기준으로 주변 2x2 영역만 검사
					if (mapGen.Tiles[i][j][k] == 1) {
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
		x = RANDOM->Int(0, mapGen.rows - 1);
		y = RANDOM->Int(0, mapGen.cols - 1);
		count = 0;
		for (int i = max(0, x - 1); i <= min(mapGen.rows - 1, x); ++i) {
			for (int j = max(0, y - 1); j <= min(mapGen.cols - 1, y); ++j) {
				// 현재 좌표 (i, j, k)의 타일을 검사
				// 수정된 부분: 중심 타일을 기준으로 주변 2x2 영역만 검사
				if (mapGen.Tiles[i][j][0] == 1) {
					count++;
				}
			}
		}
	} while (count != 4);
	float mapSize = ((mapGen.rows * mapGen.tileSize) / 2);
	player->SetSpawn(Vector3(-mapSize + x * 5, 0, -mapSize + y * 5));
}

void Scene2::Release()
{
}

void Scene2::Update()
{
	//deferred->RenderDetail();
	cam1->Find("None")->rotation = cam1->rotation;

	static bool isOnece = true;
	if (isOnece) {

		isOnece = false;
	}
	// # DEBUG #
	LIGHT->RenderDetail();
	ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());

	ImGui::Text("Player moveDir %f, %f, %f", player->MoveDir.x, player->MoveDir.y, player->MoveDir.z);

	// # DEBUG #


	ImGui::Begin("Hierarchy");
	grid->RenderHierarchy();
	cam1->RenderHierarchy();
	map->RenderHierarchy();
	Tile->RenderHierarchy();

	post->RenderHierarchy();
	player->RenderHierarchy();

	Lightting->RenderHierarchy();

	ImGui::End();

	if (INPUT->KeyDown(VK_F4)) {
		DEBUGMODE = !DEBUGMODE;
	}

	if (DEBUGMODE) {
		Camera::main->ControlMainCam();
		Camera::main = cam1;
		ResizeScreen();
		for (auto it : mapGen.WallPoint) {
			it->visible = true;
		}
	}
	else {
		Camera::main = player->Cam;
		ResizeScreen();

	}
	if (INPUT->KeyDown(VK_F5)) {
		Tile->ReleaseMember();
		Init();
	}

	grid->Update();
	map->Update();
	cam1->Update();
	Camera::main->Update();

	skybox->Update();
	skybox2->Update();
	Tile->Update();
	post->Update();

	player->Update();
	rain->Update();

	Lightting->Update();

}

void Scene2::LateUpdate()
{
	Vector3 target = player->GetWorldPos();
	static bool isOnece = false;
	if (isOnece) {
		if (!mapGen.GetTileState(target)) {
			player->GoBack();
		}
	}
	isOnece = true;





	//for (auto it : mapGen.WallPoint) {
	//	if (it->Intersect(player->Cam)) {
	//		it->visible = false;
	//	}
	//	else {
	//		it->visible = true;
	//	}
	//}
	//{
	//	//float maxDistance = 5;
	//	//Ray CameraRay;
	//	//Vector3 hit;
	//	//CameraRay.direction = player->Cam->GetWorldPos() - player->GetWorldPos();
	//	//float dis = CameraRay.direction.Length();
	//	//CameraRay.position = player->GetWorldPos();
	//	//for (auto it : mapGen.WallPoint) {
	//	//	if (it->Intersect(CameraRay, hit)) {
	//	//		float distanceToHit = (hit - CameraRay.position).Length();
	//	//		if (distanceToHit <= maxDistance) {
	//	//			Vector3 minus = hit - player->GetWorldPos();
	//	//			if (minus.Length() < dis)
	//	//			{
	//	//				// 선형 보간을 위한 비율 계산
	//	//				float t = 0.6/* 어떤 비율로 움직일지 설정하세요, 보통은 시간에 따라 변하는 값일 수 있습니다. */;
	//	//				// 현재 카메라의 위치와 목표 위치 간의 선형 보간
	//	//				Vector3 newCameraPos = Vector3::Lerp(player->Cam->GetWorldPos(), hit, t);
	//	//				// 카메라의 위치를 업데이트
	//	//				player->Cam->MoveWorldPos(newCameraPos * DELTA);
	//	//			}
	//	//			break;
	//	//		}
	//	//	}
	//	//}
	//}





	if (!DEBUGMODE) {
		//중앙값
		POINT ptMouse;
		ptMouse.x = App.GetHalfWidth();
		ptMouse.y = App.GetHalfHeight();
		Vector3 Rot;
		Rot.x = (INPUT->position.y - ptMouse.y) * 0.001f;
		Rot.y = (INPUT->position.x - ptMouse.x) * 0.001f;
		player->rotation.y += Rot.y;
		Camera::main->rotation.x += Rot.x;
		ClientToScreen(App.GetHandle(), &ptMouse);
		SetCursorPos(ptMouse.x, ptMouse.y);
	}

}

void Scene2::PreRender()
{
	Camera::main->Set();
	LIGHT->Set();

	deferred->SetTarget();

	if (not DEBUGMODE) {
		auto pCam = (Camera*)player->Cam;
		auto Cam = (Camera*)player->Cam->Find("None");
		for (auto it : mapGen.WallPoint) {
			if (Cam->Intersect(it->GetWorldPos())) {
				it->visible = false;
			}
			else {
				it->visible = true;
			}
			if (it->Intersect(pCam)) {
				it->visible = false;
			}
		}
	}


	Tile->Render();

	player->Render(RESOURCE->shaders.Load("4.Cube_Deferred.hlsl"));
	player->EffectRender();
}

void Scene2::Render()
{
	LIGHT->Set();
	Camera::main->Set();

	skybox->Render();
	skybox2->Render();

	deferred->SetRGBTexture(10);
	post->Render();

	//CamCollsion->Render();
	//map->Render();
	////auto floor = Tile->Find("floor0");
	//Tile->Render();
	//BLEND->Set(true);
	//player->Render();
	//BLEND->Set(false);
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

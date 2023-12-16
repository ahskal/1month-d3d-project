#include "stdafx.h"
#include "Player.h"
#include "Monster.h"
#include "Scene2.h"
#include "Scene1.h"

extern bool DEBUG_MODE;
extern bool NONE_SCENE;
extern bool TEXT_LOG;
extern bool FREE_CAM;

Scene1::Scene1()
{}
Scene1::~Scene1()
{
	Build->Release();
	for (auto T : map->Trees) {
		T->Release();
	}
	map->Trees.clear();
	map->Release();
	skybox->Release();
	cam1->Release();
	water->Release();
}
void Scene1::Init()
{
	water = Terrain::Create("water");
	water->LoadFile("water.xml");
	water->CreateStructuredBuffer();

	map = Terrain::Create("ground");
	map->LoadFile("ground.xml");
	map->shader = RESOURCE->shaders.Load("5.Cube_Deferred.hlsl");
	map->CreateStructuredBuffer();
	map->TreeCreateIntersect();

	skybox = Sky::Create();
	skybox->LoadFile("Sky2.xml");

	cam1 = Camera::Create();
	cam1->LoadFile("Cam.xml");

	deferred = new Deferred;

	Build = Actor::Create();
	Build->LoadFile("Object/Build.xml");

	Camera::main = cam1;
	PLAYER->MainCamSet();
	ResizeScreen();

	PLAYER->actor->SetSpawn(Vector3(-125, 10, 170));
}

void Scene1::Release()
{
}

float noise(Vector3 p, float time) {

	siv::PerlinNoise pn;
	// 물결의 스케일을 조정하기 위한 변수
	float scale = 0.3f; // 스케일 값을 조정하여 물결의 크기를 변경할 수 있습니다.

	// 시간에 따른 변화를 위해 p의 x 또는 y에 time을 더함
	p.x += time;
	p.y += time;

	// 물결의 움직임을 표현하기 위해 여러 레이어의 노이즈를 조합
	float layer1 = pn.noise3D(p.x * scale, p.y * scale, p.z * scale);
	float layer2 = pn.noise3D((p.x + 5) * scale * 2, (p.y + 5) * scale * 2, p.z * scale * 2);

	// 레이어들을 조합하여 최종 노이즈 값 계산
	float combinedNoise = (layer1 + layer2) / 2.0;

	return combinedNoise;
}

void Scene1::Update()
{
	VertexTerrain* vertices = (VertexTerrain*)water->mesh->vertices;
	float currentTime = TIMER->GetWorldTime(); // 현재 시간

	for (int i = 0; i < water->garo; i++)
	{
		for (int j = 0; j < water->garo; j++)
		{
			Vector3 point(vertices[i * water->garo + j].position);
			float result = noise(point, currentTime);
			vertices[i * water->garo + j].position.y = result;
		}
	}
	if (TIMER->GetTick(timer, 0.1f)) {
		water->UpdateNormal();
	}

	if (FREE_CAM) {
		Camera::main->ControlMainCam();
		Camera::main = cam1;
		ResizeScreen();
	}
	else {
		PLAYER->MainCamSet();
		ResizeScreen();
	}

	////
	////deferred->RenderDetail();
	//ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());
	//LIGHT->RenderDetail();
	//
	//static int num = 0;
	//if (ImGui::Button("Create House1")) {
	//	Actor* A = Actor::Create();
	//	A->LoadFile("Object/DevHouse.xml");
	//	A->name += to_string(num);
	//	Build->AddChild(A);
	//	num++;
	//}
	//
	ImGui::Begin("Hierarchy");
	cam1->RenderHierarchy();

	skybox->RenderHierarchy();
	Build->RenderHierarchy();

	map->RenderHierarchy();

	PLAYER->actor->RenderHierarchy();
	water->RenderHierarchy();
	for (auto tree : map->Trees) {
		tree->RenderHierarchy();
	}

	ImGui::End();


	Build->Update();

	cam1->Update();
	map->Update();
	for (auto tree : map->Trees) {
		tree->Update();
	}
	water->Update();
	skybox->Update();
	PLAYER->Update();
	if (PLAYER->actor->Hp < PLAYER->actor->MaxHp) {
		PLAYER->actor->Hp += DELTA;
	}

	Camera::main->Update();
}

void Scene1::LateUpdate()
{
	Ray ray;
	ray.position = PLAYER->actor->GetWorldPos() + Vector3(0, 10, 0);
	ray.direction = Vector3(0, -1, 0);
	Vector3 hit = Vector3();
	if (Utility::RayIntersectMap(ray, map, hit)) {
		PLAYER->actor->SetWorldPosY(hit.y);
	}
	PLAYER->actor->WolrdUpdate();
	if (Build->Find("IcospherePotal")->Intersect(PLAYER->actor)) {
		SCENE->AddScene("SC2", new Scene2());
		SCENE->ChangeScene("SC2");
		SCENE->DeleteScene("SC1");
		return;
	}



}
void Scene1::PreRender()
{
	LIGHT->Set();
	Camera::main->Set();
	deferred->SetTarget();
	water->Render(RESOURCE->shaders.Load("5.Cube_Deferred.hlsl"));
	map->Render(RESOURCE->shaders.Load("5.Cube_Deferred.hlsl"));
	for (auto tree : map->Trees) {
		tree->Render(RESOURCE->shaders.Load("4.Cube_Deferred.hlsl"));
	}
	PLAYER->DeferredRender(RESOURCE->shaders.Load("4.Cube_Deferred.hlsl"));
	Build->Render(RESOURCE->shaders.Load("4.Cube_Deferred.hlsl"));

}

void Scene1::Render()
{
	skybox->Render();
	deferred->Render();
	PLAYER->Render();

}

void Scene1::ResizeScreen()
{
	Camera::main->viewport.x = 0.0f;
	Camera::main->viewport.y = 0.0f;
	Camera::main->viewport.width = App.GetWidth();
	Camera::main->viewport.height = App.GetHeight();
	Camera::main->width = App.GetWidth();
	Camera::main->height = App.GetHeight();
}

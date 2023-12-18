#include "stdafx.h"
#include "Scene1.h"
#include "Scene4.h"

extern bool DEBUG_MODE;
extern bool TEXT_LOG;
extern bool FREE_CAM;
extern bool IS_PLAY;
extern bool SHOWCURSOR;

extern int ThreadCount;

float noise(Vector3 p, float time);

Scene1::Scene1() {
	skybox = Sky::Create();
	//skybox->LoadFile("Sky2.xml");
	ThreadCount++;

	cam1 = Camera::Create();
	cam1->LoadFile("Cam.xml");
	ThreadCount++;

	water = Terrain::Create("water");
	water->LoadFile("water.xml");
	water->CreateStructuredBuffer();
	ThreadCount++;

	map = Terrain::Create("ground");
	map->LoadFile("ground.xml");
	map->shader = RESOURCE->shaders.Load("5.Cube_Deferred.hlsl");
	map->CreateStructuredBuffer();
	map->TreeCreateIntersect();
	ThreadCount++;

	deferred = new Deferred;
	deferred->shader = RESOURCE->shaders.Load("6.DeferredRender2.hlsl");

	Build = Actor::Create();
	Build->LoadFile("Object/Build.xml");

	Build->Update();
	light = Light::Create();
	light->light->color.x = 1;
	light->light->color.y = 0.0588;
	light->light->color.z = 0;
	light->light->radius = 20;
	light->SetWorldPos(Build->Find("fire_pit")->GetWorldPos() + Vector3(0, 3, 0));
	ThreadCount++;

	//PLAYER->MainCamSet();
	

	
	if (!IS_PLAY) {
		FREE_CAM = true;
		DEBUG_MODE = true;
		isStart = false;
	}
	ThreadCount++;
}

Scene1::~Scene1()
{
	for (auto T : map->Trees)
	{
		T->Release();
	}
	map->Trees.clear();
	map->Release();
	water->Release();
	SafeRelease(light);	
	Build->Release();
	skybox->Release();
	cam1->Release();
}

void Scene1::Init()
{
	ThreadCount = 0;
	PLAYER->actor->SetSpawn(Vector3(-125, 20, 160));
}

void Scene1::Release() {}


void Scene1::Update()
{
	// # DEBUG #
	deferred->RenderDetail();
	// # DEBUG #
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
		SHOWCURSOR = true;

	}
	else {
		PLAYER->MainCamSet();
		ResizeScreen();
		SHOWCURSOR = false;
	}


	/*ImGui::Begin("Hierarchy");
	cam1->RenderHierarchy();

	skybox->RenderHierarchy();
	Build->RenderHierarchy();

	map->RenderHierarchy();

	PLAYER->actor->RenderHierarchy();
	water->RenderHierarchy();
	light->RenderHierarchy();

	ImGui::End();*/

	Build->Update();

	cam1->Update();
	map->Update();
	for (auto tree : map->Trees) {
		tree->Update();
	}
	water->Update();
	skybox->Update();

	static float times = 0.0f;
	if (INPUT->KeyDown(VK_SPACE)) {
		isStart = true;
		SHOWCURSOR = false;
		POINT ptMouse;
		ptMouse.x = App.GetHalfWidth();
		ptMouse.y = App.GetHalfHeight();
		ClientToScreen(App.GetHandle(), &ptMouse);
	}

	if (isStart and !IS_PLAY)
	{
		if (times < 1.0f) {
			times += DELTA / 10;
		}
		else {
			times = 2.0f;
		}
		Vector3 v1 = Camera::main->GetWorldPos();
		Vector3 v2 = static_cast<Camera*>(PLAYER->actor->Find("Camera"))->GetWorldPos();
		Vector3 Pos(1);
		Pos = v2 - v1;
		float speed = 0;
		speed = Pos.Length() * times;
		float angle = ((2 * PI) - 15 * ToRadian) - Camera::main->rotation.y;
		float moveRot = angle * times;

		float angle2 = static_cast<Camera*>(PLAYER->actor->Find("Camera"))->rotation.x - Camera::main->rotation.x;
		float moveRot2 = angle2 * times;
		Pos.Normalize();
		if (times < 1.0f) {
			Camera::main->MoveWorldPos(Pos * DELTA * speed);
			Camera::main->rotation.y += moveRot * DELTA;
			Camera::main->rotation.x += moveRot2 * DELTA;
		}
		else {
			IS_PLAY = true;
			

			FREE_CAM = false;
			DEBUG_MODE = false;
		}
	}

	light->Update();
	PLAYER->Update();
	if (IS_PLAY)
		PLAYER->UIUpdate();

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
		SCENE->AddScene("SC4", new Scene4());
		SCENE->ChangeScene("SC4");
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

	if (!isStart) {
		wstring N = L" Press the [Spacebar] to start.";
		int l = App.GetHalfWidth() - (N.length() * 8);
		int t = App.GetHalfHeight() + 76;
		int r = App.GetHalfWidth() + (N.length() * 8);
		int b = App.GetHalfHeight() + 100;
		DWRITE->RenderText(N, RECT{ l, t, r, b }, 32, L"바탕체", Color(1, 1, 1, 1));
	}


	if (IS_PLAY)
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
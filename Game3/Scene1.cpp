#include "stdafx.h"
#include "Player.h"
#include "Monster.h"
#include "Scene1.h"
extern bool DEBUG_MODE;

Scene1::Scene1()
{}
Scene1::~Scene1()
{}
void Scene1::Init()
{
	grid = Grid::Create();



	player = Player::Create();

	monster = Monster::Create();

	water = Terrain::Create("water");
	water->LoadFile("water.xml");
	water->CreateStructuredBuffer();
	water->WaterPerlinNoise();

	map = Terrain::Create("ground");
	map->LoadFile("ground.xml");
	map->CreateStructuredBuffer();
	map->GroundPerlinNoise();

	skybox = Sky::Create();
	skybox->LoadFile("Sky1.xml");
	skybox2 = Sky::Create();
	skybox2->LoadFile("Sky2.xml");

	cam1 = Camera::Create();
	cam1->LoadFile("Cam.xml");

	Camera::main = cam1;
	timer = 0;
	ResizeScreen();
}

void Scene1::Release()
{
}


float noise(Vector3 p, float time) {

	siv::PerlinNoise pn;
	// ������ �������� �����ϱ� ���� ����
	float scale = 0.1f; // ������ ���� �����Ͽ� ������ ũ�⸦ ������ �� �ֽ��ϴ�.

	// �ð��� ���� ��ȭ�� ���� p�� x �Ǵ� y�� time�� ����
	p.x += time;
	p.y += time;

	// ������ �������� ǥ���ϱ� ���� ���� ���̾��� ����� ����
	float layer1 = pn.noise3D(p.x * scale, p.y * scale, p.z * scale);
	float layer2 = pn.noise3D((p.x + 5) * scale * 2, (p.y + 5) * scale * 2, p.z * scale * 2);

	// ���̾���� �����Ͽ� ���� ������ �� ���
	float combinedNoise = (layer1 + layer2) / 2.0;

	return combinedNoise;
}

void Scene1::Update()
{
	//if (TIMER->GetTick(timer, 0.1f)) {
	//	water->WaterPerlinNoise();
	//}
	//water->mesh->UpdateBuffer();

	//VertexTerrain* vertices = (VertexTerrain*)water->mesh->vertices;

	//float currentTime = TIMER->GetWorldTime(); // ���� �ð�

	//for (int i = 0; i < water->garo; i++)
	//{
	//	for (int j = 0; j < water->garo; j++)
	//	{
	//		Vector3 point(vertices[i * water->garo + j].position);
	//		float result = noise(point, currentTime);
	//		vertices[i * water->garo + j].position.y = result;
	//	}
	//}
	//water->mesh->UpdateBuffer();
	//water->UpdateNormal();

	// ��� ���...

	LIGHT->RenderDetail();

	ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());

	if (ImGui::Button("CreateTree"))
	{
		map->TreeCreateIntersect();
	}

	if (INPUT->KeyDown(VK_F5)) {
		map->GroundPerlinNoise();
	}


	ImGui::Begin("Hierarchy");
	grid->RenderHierarchy();
	cam1->RenderHierarchy();
	player->RenderHierarchy();

	monster->Hierarchy();

	map->RenderHierarchy();
	water->RenderHierarchy();
	ImGui::End();

	cam1->ControlMainCam();


	grid->Update();
	player->Update();
	cam1->Update();

	map->Update();
	water->Update();

	skybox->Update();
	skybox2->Update();

	monster->Update();

	//// Cube�� ��ǥ���� �÷��̾��� Y�� (C)
	//float offsetY = playerPos.y - cubePos.y;
	//// �÷��̾�� ť���� ��ǥ ���̸� ��Ÿ���� ���� (C - A)
	//Vector3 dir = playerPos - cubePos;
	//// Y �� ������ ���� (ABC)
	//monster->Cube->rotation.y = atan2f(dir.x, dir.z);
	//// X �� ������ ���� (CAB) = asinf(dir.y)�� ����.
	//monster->Cube->rotation.x = -atan2f(off






}

void Scene1::LateUpdate()
{
	Ray top;
	top.position = player->GetWorldPos() + Vector3(0, 100, 0);

	top.direction = Vector3(0, -1, 0);
	Vector3 hit;
	if (Utility::RayIntersectMap(top, map, hit))
	{
		player->SetWorldPosY(hit.y);
	}


	player->WolrdUpdate();

	//for (auto it = map->Find("Node")->children.begin(); it != map->Find("Node")->children.end(); it++)
	//{
	//	for (auto it2 = it->second->children.begin(); it2 != it->second->children.end(); it2++)
	//	{
	//		for (auto it3 = it2->second->children.begin(); it3 != it2->second->children.end(); it3++)
	//		{
	//			float Length = Vector3::Distance(cam1->GetWorldPos(), it3->second->GetWorldPos());

	//			if (Length >= 300) {
	//				if (it3->second->name == "Lod3") {
	//					it3->second->visible = true;
	//				}
	//				else {
	//					it3->second->visible = false;
	//				}
	//			}
	//			else if (Length >= 150) {
	//				if (it3->second->name == "Lod1") {
	//					it3->second->visible = true;
	//				}
	//				else {
	//					it3->second->visible = false;
	//				}
	//			}
	//			else {
	//				if (it3->second->name == "Lod0") {
	//					it3->second->visible = true;
	//				}
	//				else {
	//					it3->second->visible = false;
	//				}
	//			}


}
void Scene1::PreRender()
{
	LIGHT->Set();
	cam1->Set();

	skybox->Render();
	skybox2->Render();

	cam1->Set();
	grid->Render();
	player->Render();

	map->Render();
}

void Scene1::Render()
{

	//water->Render();
	map->Render();
	//grid->Render();


	BLEND->Set(true);
	player->Render();
	BLEND->Set(false);
	monster->Render();


}

void Scene1::ResizeScreen()
{
	cam1->viewport.x = 0.0f;
	cam1->viewport.y = 0.0f;
	cam1->viewport.width = App.GetWidth();
	cam1->viewport.height = App.GetHeight();
	cam1->width = App.GetWidth();
	cam1->height = App.GetHeight();
}
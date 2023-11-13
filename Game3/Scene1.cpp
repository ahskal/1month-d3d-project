#include "stdafx.h"
#include "Player.h"
#include "Scene1.h"

Scene1::Scene1()
{}
Scene1::~Scene1()
{}
void Scene1::Init()
{
	grid = Grid::Create();

	cam1 = Camera::Create();
	cam1->LoadFile("Cam.xml");

	Camera::main = cam1;
	cam1->viewport.x = 0.0f;
	cam1->viewport.y = 0.0f;
	cam1->viewport.width = App.GetWidth();
	cam1->viewport.height = App.GetHeight();
	cam1->width = App.GetWidth();
	cam1->height = App.GetHeight();

	player = Player::Create();

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


	timer = 0;
}

void Scene1::Release()
{
}


float noise(Vector3 p, float time) {

	siv::PerlinNoise pn;
	// 물결의 스케일을 조정하기 위한 변수
	float scale = 0.1f; // 스케일 값을 조정하여 물결의 크기를 변경할 수 있습니다.

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
	//if (TIMER->GetTick(timer, 0.1f)) {
	//	water->WaterPerlinNoise();
	//}
	//water->mesh->UpdateBuffer();

	//VertexTerrain* vertices = (VertexTerrain*)water->mesh->vertices;

	//float currentTime = TIMER->GetWorldTime(); // 현재 시간

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

	// 결과 사용...

	ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());

	if (ImGui::Button("CreateTree"))
	{
		map->TreeCreateIntersect();
	}


	ImGui::Begin("Hierarchy");
	grid->RenderHierarchy();
	cam1->RenderHierarchy();
	player->RenderHierarchy();
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


	//		}
	//	}
	//}
}
void Scene1::PreRender()
{
	/*LIGHT->Set();

	skybox->Render();
	skybox2->Render();

	cam1->Set();
	grid->Render();
	player->Render();
	map->Render();*/
}

void Scene1::Render()
{
	LIGHT->Set();
	cam1->Set();

	skybox->Render();
	skybox2->Render();

	/*if (cam1->Intersect(map->GetWorldPos())) {

	}
	if (cam1->Intersect(water->GetWorldPos())) {

	}*/
	if (map->Find("Node")) {

		for (auto it = map->Find("Node")->children.begin(); it != map->Find("Node")->children.end(); it++)
		{
			if (cam1->Intersect(it->second->GetWorldPos())) {
				it->second->visible = true;
			}
			else {
				it->second->visible = false;
			}
		}
	}
	water->Render();
	map->Render();
	//grid->Render();
	player->Render();
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
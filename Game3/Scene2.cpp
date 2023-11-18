#include "stdafx.h"
#include "Scene2.h"


Scene2::Scene2()
{
	grid = Grid::Create();

	cam1 = Camera::Create();
	cam1->LoadFile("Cam2.xml");

	CamCollsion = Actor::Create();
	CamCollsion->LoadFile("Sphere.xml");

	//cam1->AddChild(CamCollsion);
	Camera::main = cam1;


	map = Terrain::Create("ground");
	// map->material->normalMap = RESOURCE->textures.Load("concrete_modular_1x1.png");
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
}

Scene2::~Scene2()
{
}

void Scene2::Init()
{
	int rows = 20;
	int cols = 20;
	int floors = 1;


	// Create an instance of MapGenerator
	MapGenerator mapGen(rows, cols, floors);

	mapGen.generateInitialMap();
	//mapGen.updateTiles();
	mapGen.checkConnectivity();
	mapGen.finalizeMap(Tile);
	mapGen.WallCreateMap(Tile);

	
	Tile->Update();

}

void Scene2::Release()
{
}

void Scene2::Update()
{
	cam1->Find("None")->rotation = cam1->rotation;

	static bool isOnece = true;
	if (isOnece) {
		
		isOnece = false;
	}
	ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());
	ImGui::Begin("Hierarchy");
	grid->RenderHierarchy();
	cam1->RenderHierarchy();
	map->RenderHierarchy();
	CamCollsion->RenderHierarchy();
	Tile->RenderHierarchy();
	ImGui::End();

	if (INPUT->KeyDown(VK_F5)) {
		Tile->ReleaseMember();
		Init();
	}

	Camera::main->ControlMainCam();

	grid->Update();
	map->Update();
	cam1->Update();
	Camera::main->Update();

	skybox->Update();
	skybox2->Update();

	
	CamCollsion->Update();

}

void Scene2::LateUpdate()
{
}

void Scene2::PreRender()
{
	LIGHT->Set();
	Camera::main->Set();

	skybox->Render();
	skybox2->Render();

}

void Scene2::Render()
{
	LIGHT->Set();
	Camera::main->Set();

	skybox->Render();
	skybox2->Render();

	CamCollsion->Render();
	map->Render();

	for (auto it = Tile->children.begin(); it != Tile->children.end(); it++) {
		Vector3 pos = it->second->GetWorldPos();
		Vector3 CamPos = cam1->Find("None")->GetWorldPos();
		
		float length = (CamPos - pos).Length();

		if (length < 40) {
			it->second->visible = true;
		}
		else {
			it->second->visible = false;
		}
		
		
	}
	Tile->Render();


}

void Scene2::ResizeScreen()
{
}

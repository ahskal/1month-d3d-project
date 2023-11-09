#include "stdafx.h"
#include "Player.h"
#include "Main.h"

Main::Main()
{

}

Main::~Main()
{

}


void Main::Init()
{
	/*grid = Grid::Create();

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

	map = Terrain::Create();
	map->LoadFile("T1.xml");
	map->CreateStructuredBuffer();



	skybox = Sky::Create();
	skybox->LoadFile("Sky1.xml");
	skybox2 = Sky::Create();
	skybox2->LoadFile("Sky2.xml");*/

}

void Main::Release()
{
}

void Main::Update()
{
	ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());
	/*ImGui::Begin("Hierarchy");
	grid->RenderHierarchy();
	cam1->RenderHierarchy();
	player->RenderHierarchy();
	map->RenderHierarchy();
	ImGui::End();

	cam1->ControlMainCam();


	grid->Update();
	player->Update();
	cam1->Update();
	map->Update();
	skybox->Update();
	skybox2->Update();*/
}

void Main::LateUpdate()
{
	/*Ray top;
	top.position = player->GetWorldPos() + Vector3(0, 100, 0);
	top.direction = Vector3(0, -1, 0);
	Vector3 hit;
	if (Utility::RayIntersectMap(top, map, hit))
	{
		player->SetWorldPosY(hit.y);
	}
	player->WolrdUpdate();*/
}
void Main::PreRender()
{
	/*LIGHT->Set();

	skybox->Render();
	skybox2->Render();

	cam1->Set();
	grid->Render();
	player->Render();
	map->Render();*/
}

void Main::Render()
{
	/*LIGHT->Set();

	skybox->Render();
	skybox2->Render();

	cam1->Set();
	grid->Render();
	map->Render();
	player->Render();*/
}

void Main::ResizeScreen()
{
	cam1->viewport.x = 0.0f;
	cam1->viewport.y = 0.0f;
	cam1->viewport.width = App.GetWidth();
	cam1->viewport.height = App.GetHeight();
	cam1->width = App.GetWidth();
	cam1->height = App.GetHeight();
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
	App.SetAppName(L"Game3");
	App.SetInstance(instance);
	WIN->Create();
	D3D->Create();
	Main* main = new Main();
	main->Init();

	int wParam = (int)WIN->Run(main);


	main->Release();
	SafeDelete(main);
	D3D->DeleteSingleton();
	WIN->DeleteSingleton();

	return wParam;
}
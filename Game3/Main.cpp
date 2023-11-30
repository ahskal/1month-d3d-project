#include "stdafx.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "ItemTable.h"

#include "Main.h"


bool DEBUG_MODE = false;

Main::Main()
{



}

Main::~Main()
{

}

void Main::Init()
{
	LIGHT->dirLight.color = Color(0, 0, 0);
	//SCENE->AddScene("SC1", new Scene1);
	SCENE->AddScene("SC2", new Scene2);
	SCENE->AddScene("SC3", new Scene3);
	SCENE->ChangeScene("SC3");
}

void Main::Release()
{

}

void Main::Update()
{
	SCENE->Update();
	/*if (INPUT->KeyDown(VK_F1)) {
		SCENE->ChangeScene("SC1");
	}*/
	if (INPUT->KeyDown(VK_F2)) {
		SCENE->ChangeScene("SC2");
	}
	if (INPUT->KeyDown(VK_F3)) {
		SCENE->ChangeScene("SC3");
	}

	if (INPUT->KeyDown(VK_F8) or INPUT->KeyDown(VK_XBUTTON1)) {
		DEBUG_MODE = !DEBUG_MODE;
	}
	if (INPUT->KeyDown(VK_XBUTTON2)) {
		PostQuitMessage(0);
	}
}

void Main::LateUpdate()
{
	SCENE->LateUpdate();

}
void Main::PreRender()
{
	SCENE->PreRender();
}

void Main::Render()
{
	SCENE->Render();
}

void Main::ResizeScreen()
{
	SCENE->ResizeScreen();
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
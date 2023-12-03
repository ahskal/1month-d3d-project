#include "stdafx.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "ItemTable.h"
#include "Inventory.h"

#include "Main.h"

bool DEBUG_MODE = false;
bool NONE_SCENE = false;
bool TEXT_LOG   = false;
bool FREE_CAM   = false;

Main::Main()
{



}

Main::~Main()
{

}

void Main::Init()
{
	if (not NONE_SCENE) {
		LIGHT->dirLight.color = Color(0, 0, 0);
		SCENE->AddScene("SC1", new Scene1);
		//eSCENE->AddScene("SC2", new Scene2);
		//eSCENE->AddScene("SC3", new Scene3);
		SCENE->ChangeScene("SC1");
	}

	Inventory inventory;

	ITEM->AddItemInfo("Gold", 100.0f);
	ITEM->AddItemInfo("Stone", 10.0f);

	ITEM->AddItemInfo("Bone", 10.0f);
	ITEM->AddItemInfo("Trash", 1.0f);
	ITEM->AddItemInfo("Leather", 30.0f);
	ITEM->AddItemInfo("Scrap", 100.0f);

	//ITEM->OpenList();
	//
	//inventory.AddItem(ITEM->CreateItem("쓰레기", new RootItemFactory));
	//inventory.AddItem(ITEM->CreateItem("가죽", new RootItemFactory));
	//inventory.AddItem(ITEM->CreateItem("돌", new RootItemFactory));
	//inventory.AddItem(ITEM->CreateItem("돌", new RootItemFactory));
	//inventory.AddItem(ITEM->CreateItem("돌", new RootItemFactory));
	//
	//cout << "##############################################" << endl;
	//
	//inventory.OpenList();

}

void Main::Release()
{

}

void Main::Update()
{


	if (not NONE_SCENE)
		SCENE->Update();
	/*if (INPUT->KeyDown(VK_F1)) {
		SCENE->ChangeScene("SC1");
	}*/
	/*if (INPUT->KeyDown(VK_F2)) {
		if (not None_Scene)
			SCENE->ChangeScene("SC2");
	}
	if (INPUT->KeyDown(VK_F3)) {
		if (not None_Scene)
			SCENE->ChangeScene("SC3");
	}*/

	if (INPUT->KeyDown(VK_F6)) {
		DEBUG_MODE = !DEBUG_MODE;
	}
	if (INPUT->KeyDown(VK_F7)) {
		FREE_CAM = !FREE_CAM;
	}
	if (INPUT->KeyDown(VK_F8)) {
		TEXT_LOG = !TEXT_LOG;
	}
	if (INPUT->KeyDown(VK_XBUTTON1)) {
		DEBUG_MODE = !DEBUG_MODE;
		FREE_CAM = !FREE_CAM;
	}

	if (INPUT->KeyDown(VK_MBUTTON)) {
		PostQuitMessage(0);
	}
}

void Main::LateUpdate()
{
	if (not NONE_SCENE)
		SCENE->LateUpdate();

}
void Main::PreRender()
{
	if (not NONE_SCENE)
		SCENE->PreRender();
}

void Main::Render()
{
	if (not NONE_SCENE)
		SCENE->Render();
}

void Main::ResizeScreen()
{
	if (not NONE_SCENE)
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
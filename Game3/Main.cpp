#include "stdafx.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "ItemTable.h"
#include "Inventory.h"

#include "Main.h"

bool DEBUG_MODE = false;
bool TEXT_LOG = false;
bool FREE_CAM = false;

bool IS_PLAY = false;
bool SHOWCURSOR = true;

bool WIREFRAME = false;


Main::Main()
{

}

Main::~Main()
{

}

void Main::Init()
{

	LIGHT->dirLight.color = Color(0, 0, 0);
	SCENE->AddScene("SC1", new Scene1);
	//SCENE->AddScene("SC3", new Scene3);
	//SCENE->AddScene("SC4", new Scene4);
	SCENE->ChangeScene("SC1");

	Inventory inventory;

	ITEM->AddItemInfo("Gold", 100.0f);
	//ITEM->AddItemInfo("Stone", 10.0f);
	//
	//ITEM->AddItemInfo("Bone", 10.0f);
	//ITEM->AddItemInfo("Trash", 1.0f);
	//ITEM->AddItemInfo("Leather", 30.0f);
	//ITEM->AddItemInfo("Scrap", 100.0f);

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

	ShowCursor(SHOWCURSOR);

	SCENE->Update();

	if (INPUT->KeyDown(VK_F6)) {

	}
	if (INPUT->KeyDown(VK_F7)) {

	}
	if (INPUT->KeyDown(VK_F8)) {
		WIREFRAME = !WIREFRAME;
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
	SCENE->LateUpdate();

}
void Main::PreRender()
{
	if (WIREFRAME)
		RASTER->Set(D3D11_CULL_FRONT, D3D11_FILL_WIREFRAME);
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
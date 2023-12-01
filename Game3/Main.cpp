#include "stdafx.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "ItemTable.h"
#include "Inventory.h"

#include "Main.h"


bool DEBUG_MODE = false;
bool FREE_CAM = false;;
bool None_Scene = false;;

Main::Main()
{



}

Main::~Main()
{

}

void Main::Init()
{
	if (not None_Scene) {
		LIGHT->dirLight.color = Color(0, 0, 0);
		//SCENE->AddScene("SC1", new Scene1);
		SCENE->AddScene("SC2", new Scene2);
		//SCENE->AddScene("SC3", new Scene3);
		SCENE->ChangeScene("SC2");
	}

	Inventory inventory;

	ITEM->AddItemInfo("½ºÄÌ·¹Åæ »À", 10.0f);
	ITEM->AddItemInfo("¾²·¹±â", 1.0f);
	ITEM->AddItemInfo("¾²·¹±â", 1.0f);
	ITEM->AddItemInfo("°¡Á×", 30.0f);
	ITEM->AddItemInfo("°¡Á×", 30.0f);
	ITEM->AddItemInfo("°¡Á×", 30.0f);
	ITEM->AddItemInfo("µ¹", 100.0f);
	ITEM->AddItemInfo("ÀÌ¸§ ¸ð¸¦ °¡·ç", 10.0f);
	ITEM->OpenList();

	inventory.AddItem(ITEM->CreateItem("¾²·¹±â", new RootItemFactory));
	inventory.AddItem(ITEM->CreateItem("°¡Á×", new RootItemFactory));
	inventory.AddItem(ITEM->CreateItem("µ¹", new RootItemFactory));
	inventory.AddItem(ITEM->CreateItem("µ¹", new RootItemFactory));
	inventory.AddItem(ITEM->CreateItem("µ¹", new RootItemFactory));

	cout << "##############################################" << endl;

	inventory.OpenList();

}

void Main::Release()
{

}

void Main::Update()
{


	if (not None_Scene)
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

	if (INPUT->KeyDown(VK_F8) or INPUT->KeyDown(VK_XBUTTON1)) {
		DEBUG_MODE = !DEBUG_MODE;
	}
	if (INPUT->KeyDown(VK_XBUTTON2)) {
		FREE_CAM = !FREE_CAM;
	}

	if (INPUT->KeyDown(VK_MBUTTON)) {
		PostQuitMessage(0);
	}
}

void Main::LateUpdate()
{
	if (not None_Scene)
		SCENE->LateUpdate();

}
void Main::PreRender()
{
	if (not None_Scene)
		SCENE->PreRender();
}

void Main::Render()
{
	if (not None_Scene)
		SCENE->Render();
}

void Main::ResizeScreen()
{
	if (not None_Scene)
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
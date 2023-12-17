#include "stdafx.h"
#include "Scene4.h"
#include "Scene2.h"

extern bool DEBUG_MODE;
extern bool TEXT_LOG;
extern bool FREE_CAM;
extern bool IS_PLAY;
extern bool SHOWCURSOR;

int ThreadCount2 = 0;

void CreateScene2() {
	SCENE->AddScene("SC2", new Scene2);
}

Scene4::Scene4()
{
	deferred = new Deferred;

	BG = UI::Create();

	cam = Camera::Create();

	Camera::main = cam;
	Thread = new thread(CreateScene2);
}

Scene4::~Scene4()
{
	Thread->join();
	delete Thread;
}

void Scene4::Init()
{
	SCENE->DeleteScene("SC1");

}

void Scene4::Release()
{

}

void Scene4::Update()
{
	LIGHT->RenderDetail();
	ImGui::Begin("Hierarchy", nullptr);
	BG->RenderHierarchy();

	ImGui::End();
	Camera::main->Update();
	BG->Update();
}

void Scene4::LateUpdate()
{
}

void Scene4::PreRender()
{
	LIGHT->Set();
	Camera::main->Set();
	deferred->SetTarget();
	BG->Render();

	if (ThreadCount2 == 12)
	{
		SCENE->ChangeScene("SC2");
		SCENE->DeleteScene("SC4");
		ThreadCount2 = 0;
	}
}

void Scene4::Render()
{
	deferred->Render();
	wstring N = L" Loading...";
	int l = App.GetHalfWidth() - (N.length() * 8);
	int t = App.GetHalfHeight() + 76;
	int r = App.GetHalfWidth() + (N.length() * 8);
	int b = App.GetHalfHeight() + 100;
	DWRITE->RenderText(N, RECT{ l, t, r, b }, 32, L"¹ÙÅÁÃ¼", Color(1, 1, 1, 1));
}

void Scene4::ResizeScreen()
{
	Camera::main->viewport.x = 0.0f;
	Camera::main->viewport.y = 0.0f;
	Camera::main->viewport.width = App.GetWidth();
	Camera::main->viewport.height = App.GetHeight();
	Camera::main->width = App.GetWidth();
	Camera::main->height = App.GetHeight();
}

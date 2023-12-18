#include "stdafx.h"
#include "Scene3.h"
#include "Scene1.h"

extern bool DEBUG_MODE;
extern bool TEXT_LOG;
extern bool FREE_CAM;
extern bool IS_PLAY;
extern bool SHOWCURSOR;


int ThreadCount = 0;

void CreateScene1() {
	SCENE->AddScene("SC1", new Scene1);
}

Scene3::Scene3()
{
	deferred = new Deferred;

		BG = UI::Create();

	cam = Camera::Create();

	Camera::main = cam;
	Thread = new thread(CreateScene1);
}

Scene3::~Scene3()
{
	Thread->join();
	delete Thread;
}

void Scene3::Init()
{
	SCENE->DeleteScene("SC2");

}

void Scene3::Release() {

}

void Scene3::Update()
{
	////LIGHT->RenderDetail();
	//ImGui::Begin("Hierarchy", nullptr);
	//BG->RenderHierarchy();

	//ImGui::End();
	//Camera::main->Update();
	//BG->Update();

	if (ThreadCount == 6)
	{
		SCENE->ChangeScene("SC1");
		SCENE->DeleteScene("SC3");
	}

}

void Scene3::LateUpdate()
{

}

void Scene3::PreRender()
{
	LIGHT->Set();
	Camera::main->Set();
	deferred->SetTarget();
	BG->Render();

}

void Scene3::Render()
{
	deferred->Render();
	wstring N = L" Loading...";
	int l = App.GetHalfWidth() - (N.length() * 8);
	int t = App.GetHalfHeight() + 76;
	int r = App.GetHalfWidth() + (N.length() * 8);
	int b = App.GetHalfHeight() + 100;
	DWRITE->RenderText(N, RECT{ l, t, r, b }, 32, L"¹ÙÅÁÃ¼", Color(1, 1, 1, 1));
}

void Scene3::ResizeScreen()
{
	Camera::main->viewport.x = 0.0f;
	Camera::main->viewport.y = 0.0f;
	Camera::main->viewport.width = App.GetWidth();
	Camera::main->viewport.height = App.GetHeight();
	Camera::main->width = App.GetWidth();
	Camera::main->height = App.GetHeight();
}

#include "stdafx.h"

#include "PlayerData.h"
#include "Player.h"
#include "UI_Player.h"
#include "PlayerObserver.h"

#include "Item.h"

#include "Scene3.h"

extern bool DEBUG_MODE;
extern bool NONE_SCENE;
extern bool TEXT_LOG;
extern bool FREE_CAM;

Scene3::Scene3()
{
	deferred = new Deferred;
	post = UI::Create();
	post->LoadFile("Deferred.xml");
}

Scene3::~Scene3()
{
}

void Scene3::Init()
{

}

void Scene3::Release() {}

void Scene3::Update()
{

	LIGHT->RenderDetail();

	ImGui::Begin("Hierarchy", nullptr);

	PLAYER->Hierarchy();



	ImGui::End();

	post->Update();


	PLAYER->Update();
	Camera::main->Update();
}

void Scene3::LateUpdate()
{
	ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());

	PLAYER->LateUpdate();
}

void Scene3::PreRender()
{
	LIGHT->Set();
	Camera::main->Set();

	deferred->SetTarget();

	PLAYER->DeferredRender(RESOURCE->shaders.Load("4.Cube_Deferred.hlsl"));

}

void Scene3::Render()
{
	deferred->Render();

	PLAYER->Render();
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

#include "stdafx.h"
#include "Scene4.h"

Scene4::Scene4()
{
	deferred = new Deferred;
}

Scene4::~Scene4()
{
}

void Scene4::Init()
{
}

void Scene4::Release()
{
}

void Scene4::Update()
{
}

void Scene4::LateUpdate()
{
}

void Scene4::PreRender()
{
	deferred->SetTarget();

}

void Scene4::Render()
{
	deferred->Render();
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

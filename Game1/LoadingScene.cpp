#include "stdafx.h"
#include "LoadingScene.h"
#include "Scene1.h"

wstring loadObejct;
int     loadCount = 0;

void CreateScene1()
{
    SCENE->AddScene("SC1", new Scene1);
}

void LoadingScene::Init()
{
    grid = Grid::Create();

    cam1 = Camera::Create();
    cam1->LoadFile("Cam.xml");
    Camera::main = cam1;

    bono = UI::Create();
    bono->LoadFile("UI.xml");

    t1 = new thread(CreateScene1);
}

void LoadingScene::Release()
{
    t1->join();
    delete t1;
}

void LoadingScene::Update()
{
    Camera::main->ControlMainCam();
    Camera::main->Update();

    ImGui::Begin("Hierarchy");
    grid->RenderHierarchy();
    cam1->RenderHierarchy();
    bono->RenderHierarchy();
    ImGui::End();
    grid->Update();
    cam1->Update();
    bono->Update();

    if (loadCount == 4)
    {
        SCENE->ChangeScene("SC1");
    }

}

void LoadingScene::LateUpdate()
{
}

void LoadingScene::PreRender()
{
}

void LoadingScene::Render()
{
    DWRITE->RenderText(loadObejct, RECT{ 0,0,1000,300 }, 50, L"¹ÙÅÁÃ¼", Color(1, 0, 0, 1));
    cam1->Set();
    LIGHT->Set();
    grid->Render();
    bono->Render();
}

void LoadingScene::ResizeScreen()
{
}

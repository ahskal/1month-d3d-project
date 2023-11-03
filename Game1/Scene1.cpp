#include "stdafx.h"
#include "Fruit.h"
#include "Zamong.h"
#include "Mutant.h"
#include "Scene1.h"

extern wstring loadObejct;
extern int     loadCount;


Scene1::Scene1()
{
    loadObejct = L"Grid";  
    grid = Grid::Create();
    loadCount++;

    loadObejct = L"Cam";
    cam1 = Camera::Create();
    cam1->LoadFile("Cam.xml");



    Camera::main = cam1;
    loadCount++;

    loadObejct = L"Terrain"; 
    map = Terrain::Create();
    map->LoadFile("Terrain.xml");
    map->CreateStructuredBuffer();
    loadCount++;

    loadObejct = L"Vam";
    player = Zamong::Create("Vam");

    skybox = Sky::Create();
    skybox->LoadFile("Sky1.xml");
    skybox2 = Sky::Create();
    skybox2->LoadFile("Sky2.xml");

    sphere = Actor::Create();
    sphere->LoadFile("Sphere.xml");

    mutant = new Mutant();

    environmentMapSize = 256;
    environmentMap = new CubeRenderTarget(256);

    shadowMapSize = 256;
    shadowMap = new CubeRenderTarget(256);

    rain = Rain::Create();


    RESOURCE->shaders.Load("0.Sky_CR.hlsl")->LoadGeometry();
    RESOURCE->shaders.Load("0.SkySphere_CR.hlsl")->LoadGeometry();
    RESOURCE->shaders.Load("5.Cube_CR.hlsl")->LoadGeometry();
    RESOURCE->shaders.Load("5.Cube_Shadow.hlsl")->LoadGeometry();
    RESOURCE->shaders.Load("4.Instance_CR.hlsl")->LoadGeometry();
    RESOURCE->shaders.Load("4.Instance_Shadow.hlsl")->LoadGeometry();


    loadCount++;
}
Scene1::~Scene1()
{

}

void Scene1::Init()
{
   
    //player->LoadFile("Vam.xml");
}

void Scene1::Release()
{
}

void Scene1::Update()
{
    rain->Gui();
    //mutant->slash->RenderDetail();
    //shadowMap->RenderDetail();


    LIGHT->RenderDetail();

    Camera::main->ControlMainCam();
    Camera::main->Update();
    ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());
    ImGui::Begin("Hierarchy");
    grid->RenderHierarchy();
    cam1->RenderHierarchy();
    map->RenderHierarchy();
    player->RenderHierarchy();
    skybox->RenderHierarchy();
    skybox2->RenderHierarchy();
    sphere->RenderHierarchy();
    mutant->actor->RenderHierarchy();
    rain->RenderHierarchy();
    ImGui::End();


    grid->Update();
    map->Update();
    player->Update();
    skybox->Update();
    skybox2->Update();
    sphere->Update();
    mutant->Update();
    rain->Update();
    //sphere[0][0][0]->Update();

}

void Scene1::LateUpdate()
{
    Ray top;
    top.position = player->GetWorldPos() + Vector3(0, 100, 0);
    top.direction = Vector3(0, -1, 0);
    Vector3 hit;
    if (Utility::RayIntersectMap(top, map, hit))
    {
        player->SetWorldPosY(hit.y);
    }
    player->WolrdUpdate();
    Vector3 dir = player->GetWorldPos() - player->GetLast();
    Vector3 dir2 = dir;
    dir2.y = 0;
    dir.Normalize();
    dir2.Normalize();
    float dot = dir.Dot(dir2);
    //cout << dot << endl;
    if (player->GetState() == ZamongState::MOVE)
    {
        if (dot < 0.7 and (player->GetWorldPos().y > player->GetLast().y))
        {
            player->SetWorldPos(player->GetLast());
            player->WolrdUpdate();
        }
    }

    if (INPUT->KeyPress(VK_RBUTTON))
    {
        Ray zamongRay = Utility::MouseToRay();

        Vector3 hit;
        //광선충돌시에
        GameObject* temp = map;
        //광선충돌시에
        //if (Utility::RayIntersectTri(zamongRay, map, hit))
        /*if (map->ComPutePicking(zamongRay,hit))
        {
            player->SetWorldPos(hit);
        }*/
    }
    if (INPUT->KeyDown('Q'))
    {
        

    }
}

void Scene1::PreRender()
{
    //텍스쳐 위에 그리기

    static int captureCount = 0;
    LIGHT->Set();
    if (captureCount == 0)
    {
       
        captureCount++;
    }

    environmentMap->position = sphere->GetWorldPos();
    environmentMap->SetTarget();
    //육면체 텍스처 렌더링용 쉐이더
    skybox2->Render(RESOURCE->shaders.Load("0.SkySphere_CR.hlsl"));  //배경
    skybox->Render(RESOURCE->shaders.Load("0.Sky_CR.hlsl"));
    map->Render(RESOURCE->shaders.Load("5.Cube_CR.hlsl"));
    player->Render(RESOURCE->shaders.Load("4.Instance_CR.hlsl"));
   
    //shadowMap->position = player->GetWorldPos() - LIGHT->dirLight.direction * 50.0f;
    shadowMap->position = sphere->GetWorldPos();;
    shadowMap->SetTarget();
    //map->Render(RESOURCE->shaders.Load("5.Cube_Shadow.hlsl"));
    player->Render(RESOURCE->shaders.Load("4.Instance_Shadow.hlsl"));
    mutant->Render(RESOURCE->shaders.Load("4.Instance_Shadow.hlsl"));

} 

void Scene1::Render()
{
    Camera::main->Set();
    LIGHT->Set();
   
   
    skybox->Render();
    skybox2->Render();

    grid->Render();
    //그림자맵 텍스처
    shadowMap->SetRGBTexture(5);
    map->Render();

    player->Render();
    mutant->Render();

    //환경맵 텍스처
    environmentMap->SetRGBTexture(4);
    sphere->Render();
   
    rain->Render();
}

void Scene1::ResizeScreen()
{
    cam1->viewport.x = 0.0f;
    cam1->viewport.y = 0.0f;
    cam1->viewport.width = App.GetWidth();
    cam1->viewport.height = App.GetHeight();

    cam1->width = App.GetWidth();
    cam1->height = App.GetHeight();
}

#include "stdafx.h"
#include "Scene2.h"

Scene2::Scene2()
{
	grid = Grid::Create();

	cam1 = Camera::Create();
	cam1->LoadFile("Cam.xml");
	Camera::main = cam1;

	map = Terrain::Create("ground");
	map->CreateStructuredBuffer();
	map->TreeCreateIntersect();

}

Scene2::~Scene2()
{
}


void Scene2::Init()
{
    //constexpr int width = 100;
    //constexpr int height = 100;
    //Scene2 biomeGenerator;
    //// 바이옴과 나무를 생성하고 출력
    //for (int i = 0; i < width; ++i) {
    //    for (int j = 0; j < height; ++j) {
    //        Biome biome = biomeGenerator.getBiome(i, j);
    //        // 바이옴에 따라 출력을 다르게 설정
    //        switch (biome) {
    //        case Biome::Plains:
    //            std::cout << '.';
    //            break;
    //        case Biome::Mountains:
    //            std::cout << '^';
    //            break;
    //        case Biome::Desert:
    //            std::cout << '~';
    //            break;
    //        case Biome::Forest:
    //            std::cout << 'T';  // 나무가 있는 지역을 표시
    //            break;
    //        }
    //    }
    //    std::cout << std::endl;
    //}
}

void Scene2::Release()
{
}

void Scene2::Update()
{
	ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());
	ImGui::Begin("Hierarchy");
	grid->RenderHierarchy();
	cam1->RenderHierarchy();
	map->RenderHierarchy();
	water->RenderHierarchy();
	ImGui::End();

	Camera::main->ControlMainCam();

	grid->Update();
	map->Update();
	water->Update();
	Camera::main->Update();
}

void Scene2::LateUpdate()
{
}

void Scene2::PreRender()
{
}

void Scene2::Render()
{
	LIGHT->Set();
	Camera::main->Set();
	//grid->Render();
	water->Render();
	map->Render();
}

void Scene2::ResizeScreen()
{
}

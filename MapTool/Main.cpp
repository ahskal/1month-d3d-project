#include "stdafx.h"
#include "Main.h"

Main::Main()
{
	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = sizeof(Brush);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &brushBuffer);
	assert(SUCCEEDED(hr));
	D3D->GetDC()->PSSetConstantBuffers(10, 1, &brushBuffer);

	//투명 이미지출력
}

Main::~Main()
{

}


void Main::Init()
{
	grid = Grid::Create();

	cam1 = Camera::Create();
	cam1->LoadFile("Cam.xml");

	map = Terrain::Create("ground");
	map->LoadFile("ground.xml");
	map->shader = RESOURCE->shaders.Load("5.MapEditor.hlsl");
	map->CreateStructuredBuffer();

	watermap = Terrain::Create("water");
	watermap->LoadFile("water.xml");
	watermap->shader = RESOURCE->shaders.Load("5.MapEditor.hlsl");
	watermap->CreateStructuredBuffer();

	Camera::main = cam1;
	cam1->viewport.x = 0.0f;
	cam1->viewport.y = 0.0f;
	cam1->viewport.width = App.GetWidth();
	cam1->viewport.height = App.GetHeight();
	cam1->width = App.GetWidth();
	cam1->height = App.GetHeight();

	Tree = Actor::Create();
	//Wall->LoadFile("");

	TreeNum = 100;
}

void Main::Release()
{
}

void Main::Update()
{
	// ============================ GUI ==================
	ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());

	ImGui::SliderInt("Tree Max Num", &TreeNum, 1, 1000);

	if (ImGui::Button("CreateTree"))
	{
		CreateTree();
	}

	LIGHT->RenderDetail();

	ImGui::Combo("brushShape", (int*)&brushShape, "Circle\0Rect\0");
	ImGui::Combo("brushType", (int*)&brushType, "Linear\0Smooth\0Add\0Flat\0");

	ImGui::SliderFloat("BrushRange", &_brush.range, 1, 100);
	ImGui::SliderFloat("buildSpeed", &buildSpeed, -100, 100);

	if (ImGui::Button("paint0"))
	{
		paint = Vector4(1, -1, -1, -1);
	}
	if (ImGui::Button("paint1"))
	{
		paint = Vector4(-1, 1, -1, -1);
	}
	if (ImGui::Button("paint2"))
	{
		paint = Vector4(-1, -1, 1, -1);
	}
	if (ImGui::Button("paint3"))
	{
		paint = Vector4(-1, -1, -1, 1);
	}
	ImGui::SliderFloat("paintSpeed", &paintSpeed, 0.001f, 3.0f);

	// ===================================================

	ImGui::Begin("Hierarchy");
	cam1->RenderHierarchy();
	map->RenderHierarchy();
	watermap->RenderHierarchy();
	Tree->RenderHierarchy();
	ImGui::End();

	cam1->ControlMainCam();

	grid->Update();
	map->Update();
	watermap->Update();

	cam1->Update();

	Tree->Update();
}

void Main::LateUpdate()
{

	//Brush
	{
		Vector3 Hit;
		if (map->ComPutePicking(Utility::MouseToRay(), Hit))
		{
			_brush.point = Hit;

			if (INPUT->KeyPress(VK_MBUTTON))
			{
				VertexTerrain* vertices = (VertexTerrain*)map->mesh->vertices;
				Matrix Inverse = map->W.Invert();
				Hit = Vector3::Transform(Hit, Inverse);
				for (UINT i = 0; i < map->mesh->vertexCount; i++)
				{
					Vector3 v1 = Vector3(Hit.x, 0.0f, Hit.z);
					Vector3 v2 = Vector3(vertices[i].position.x,
						0.0f, vertices[i].position.z);
					Vector3 temp = v2 - v1;
					float Dis = temp.Length();
					float w;

					if (brushShape == BrushShape::Rect)
					{
						float DisX = fabs(v1.x - v2.x);
						float DisZ = fabs(v1.z - v2.z);

						if (DisX < _brush.range and
							DisZ < _brush.range)
						{
							//nomalize
							//w = Dis / (_brush.range * 1.414f);
							// 
							w = ((DisX > DisZ) ? DisX : DisZ) / _brush.range;

							// 0 ~ 1
							w = Utility::Saturate(w);
							w = (1.0f - w);
						}
						else
						{
							w = 0.0f;
						}
					}
					if (brushShape == BrushShape::Circle)
					{
						w = Dis / _brush.range;
						w = Utility::Saturate(w);
						w = (1.0f - w);
					}

					if (brushType == BrushType::Linear)
					{
						if (w)
						{
							vertices[i].position.y += w * buildSpeed * DELTA;
						}
					}
					if (brushType == BrushType::Smooth)
					{
						if (w)
						{
							vertices[i].position.y += sin(w * PI * 0.5f) * buildSpeed * DELTA;
						}
					}
					if (brushType == BrushType::Add)
					{
						if (w)
						{
							vertices[i].position.y += buildSpeed * DELTA;
						}
					}
					if (brushType == BrushType::Flat)
					{
						if (w)
						{
							float dd = vertices[i].position.y > Hit.y ? -1 : 1;
							vertices[i].position.y += w * dd * buildSpeed * DELTA;

							vertices[i].position.y = Hit.y;
						}
					}
					if (w)
					{
						vertices[i].weights += paint * w * paintSpeed * DELTA;
						float* weight = (float*)&(vertices[i].weights);
						float	sum = 0;
						for (int i = 0; i < 4; i++)
						{
							weight[i] = Utility::Saturate(weight[i]);
							sum += weight[i];
						}
						for (int i = 0; i < 4; i++)
						{
							weight[i] /= sum;
						};
					}
				}
				map->mesh->UpdateBuffer();
				map->UpdateNormal();
			}
			if (INPUT->KeyUp(VK_MBUTTON))
			{
				map->UpdateNormal();
				map->UpdateStructuredBuffer();
			}
		}
	}

	//LOD 적용 식
	for (auto it = Tree->obList.begin(); it != Tree->obList.end(); it++)
	{
		for (auto it2 = it->second->children.begin(); it2 != it->second->children.end(); it2++)
		{
			for (auto it3 = it2->second->children.begin(); it3 != it2->second->children.end(); it3++)
			{

				float Length = Vector3::Distance(cam1->GetWorldPos(), it3->second->GetWorldPos());

				if (Length >= 250) {
					if (it3->second->name == "Lod3")
					{
						it3->second->visible = true;
					}
					else {
						it3->second->visible = false;
					}
				}
				else if (Length >= 150) {
					if (it3->second->name == "Lod1")
					{
						it3->second->visible = true;
					}
					else {
						it3->second->visible = false;
					}
				}
				else if (Length >= 10) {
					if (it3->second->name == "Lod0")
					{
						it3->second->visible = true;
					}
					else {
						it3->second->visible = false;
					}
				}
			}
		}
	}


}
void Main::PreRender()
{
	_brush.shape = (int)brushShape;
	_brush.type = (int)brushType;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	D3D->GetDC()->Map(brushBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy_s(mappedResource.pData, sizeof(Brush), &_brush, sizeof(Brush));
	D3D->GetDC()->Unmap(brushBuffer, 0);

	LIGHT->Set();
	Camera::main->Set();

	//grid->Render();
	watermap->Render();
	map->Render();

	/*for (auto it = Tree->obList.begin(); it != Tree->obList.end(); it++)
	{
		if (cam2->Intersect(it->second->GetWorldPos())) {

			it->second->visible = true;
		}
		else {
			it->second->visible = false;
		}
	}
	Tree->Render();*/
}

void Main::Render()
{
	_brush.shape = (int)brushShape;

	_brush.type = (int)brushType;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	D3D->GetDC()->Map(brushBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy_s(mappedResource.pData, sizeof(Brush), &_brush, sizeof(Brush));
	D3D->GetDC()->Unmap(brushBuffer, 0);

	LIGHT->Set();
	Camera::main->Set();


	//grid->Render();
	watermap->Render();
	map->Render();
	for (auto it = Tree->obList.begin(); it != Tree->obList.end(); it++)
	{
		if (cam1->Intersect(it->second->GetWorldPos())) {

			it->second->visible = true;
		}
		else {
			it->second->visible = false;
		}
	}
	Tree->Render();

}

void Main::ResizeScreen()
{
	cam1->viewport.x = 0.0f;
	cam1->viewport.y = 0.0f;
	cam1->viewport.width = App.GetWidth();
	cam1->viewport.height = App.GetHeight();
	cam1->width = App.GetWidth();
	cam1->height = App.GetHeight();
}

void Main::CreateTree()
{
	// empty가 아니라면
	if (not Tree->obList.empty()) {
		// 즉 기존에 한번 사용한적이 있는 대상이기 때문에
		// 나무객체의 하위 자식들을 제거후 다시 생성
		Tree->ReleaseMember();
	}
	
	siv::PerlinNoise pn;
	VertexTerrain* vertices = (VertexTerrain*)map->mesh->vertices;
	int Count = 0;

	int maxTree = TreeNum;
	double halfsize = map->garo / 2;

	for (int i = 0; i < map->garo; i++)
	{
		for (int j = 0; j < map->garo; j++)
		{
			if (Count != maxTree) {

				Vector3 Pos = Vector3(RANDOM->Int(-halfsize, halfsize), 0, RANDOM->Int(-halfsize, halfsize));

				Ray heightChecker;
				heightChecker.position = Pos;
				heightChecker.direction = Vector3(0, -1, 0);

				Vector3 hit;

				Actor* Tree = Actor::Create();

				if (Utility::RayIntersectMap(heightChecker, map, hit)) {
					if (hit.y >= -20.0f) {
						int rand = RANDOM->Int();
						Tree->LoadFile("Beech.xml");

						Tree->name = "Tree" + to_string(Count);
						Tree->SetWorldPosX(Pos.x);
						Tree->SetWorldPosY(hit.y);
						Tree->SetWorldPosZ(Pos.z);
					}
				}

				this->Tree->AddChild(Tree);
				Count++;
			}
			else {
				break;
			}

		}
	}

}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
	App.SetAppName(L"MapEditor");
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
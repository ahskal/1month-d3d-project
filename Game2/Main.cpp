#include "stdafx.h"
#include "Main.h"

Main::Main()
{


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
	Camera::main = cam1;

	shader = new Shader();
	shader->LoadFile("TessRect.hlsl");
	shader->LoadTessellation();

	int vertexCount = 4;
	VertexP* vertices = new VertexP[vertexCount];
	vertices[0] = Vector3(-0.9, 0.9, 0);
	vertices[1] = Vector3(-0.9, -0.9, 0);
	vertices[2] = Vector3(0.9, 0.9, 0);
	vertices[3] = Vector3(0.9, -0.9, 0);

	int indexCount = 4;
	UINT* indices = new UINT[indexCount];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;

	mesh = new Mesh(vertices, vertexCount, indices, indexCount, VertexType::P);
	mesh->primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;

	edgeBufferDesc.edges.x = 1;
	edgeBufferDesc.edges.y = 1;
	edgeBufferDesc.edges.z = 1;
	edgeBufferDesc.edges.w = 1;
	edgeBufferDesc.inside.x = 1;
	edgeBufferDesc.inside.y = 1;

	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = sizeof(EdgeBuffer);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &edgeBuffer);
		assert(SUCCEEDED(hr));

	}

}

void Main::Release()
{
}

void Main::Update()

{
	ImGui::Begin("Hierarchy");
	grid->RenderHierarchy();
	ImGui::End();

	cam1->ControlMainCam();

	cam1->Update();
	grid->Update();
}

void Main::LateUpdate()
{
	ImGui::DragFloat("Edge0", &edgeBufferDesc.edges.x);
	ImGui::DragFloat("Edge1", &edgeBufferDesc.edges.y);
	ImGui::DragFloat("Edge2", &edgeBufferDesc.edges.z);
	ImGui::DragFloat("Edge3", &edgeBufferDesc.edges.w);
	ImGui::DragFloat("Inside0", &edgeBufferDesc.inside.x);
	ImGui::DragFloat("Inside1", &edgeBufferDesc.inside.y);

}
void Main::PreRender()
{
}

void Main::Render()
{
	cam1->Set();
	LIGHT->Set();

	grid->Render();

	RASTER->Set(D3D11_CULL_NONE, D3D11_FILL_WIREFRAME);
	{
		//상수버퍼
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D->GetDC()->Map(edgeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(EdgeBuffer), &edgeBufferDesc, sizeof(EdgeBuffer));
		D3D->GetDC()->Unmap(edgeBuffer, 0);
		D3D->GetDC()->HSSetConstantBuffers(10, 1, &edgeBuffer);
	}
	shader->Set();
	mesh->Set();
	D3D->GetDC()->Draw(4, 0);

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

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
	App.SetAppName(L"Game2");
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
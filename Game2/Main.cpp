#include "stdafx.h"
#include "Main.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

void Main::ReadMtl(string file)
{
	ifstream fin;
	fin.open("../Contents/" + file);
	if (fin.is_open())
	{
		string name;
		while (!fin.eof())
		{
			string Input;
			fin >> Input;
			if (Input == "newmtl")
			{
				fin >> name;
				mtlList[name] = new  Material();
			}
			else if (Input == "Ka")
			{
				fin >> mtlList[name]->ambient.x;
				fin >> mtlList[name]->ambient.y;
				fin >> mtlList[name]->ambient.z;
			}
			else if (Input == "Kd")
			{
				fin >> mtlList[name]->diffuse.x;
				fin >> mtlList[name]->diffuse.y;
				fin >> mtlList[name]->diffuse.z;
			}
			else if (Input == "Ks")
			{
				fin >> mtlList[name]->specular.x;
				fin >> mtlList[name]->specular.y;
				fin >> mtlList[name]->specular.z;
			}
			else if (Input == "map_Kd")
			{
				mtlList[name]->diffuse.w = 1.0f;
				string TexName;
				fin >> TexName;

				{
					size_t tok = file.find_last_of(".");
					string filename = file.substr(0, tok);
					string checkPath = "../Contents/Texture/" + filename;
					//이경로가 있는지 체크
					if (!PathFileExistsA(checkPath.c_str()))
					{
						//없으면 만들어라
						CreateDirectoryA(checkPath.c_str(), NULL);
					}
					string orgin = "../Assets/" + TexName;
					string copy = "../Contents/Texture/" + file.substr(0, tok) + "/" + TexName;
					bool isCheck = true;
					CopyFileA(orgin.c_str(), copy.c_str(), isCheck);
					mtlList[name]->diffuseMap = make_shared<Texture>();
					mtlList[name]->diffuseMap->LoadFile(TexName);
					//temp->diffuse.w = 1.0f;
				}
				{
					size_t tok = file.find_last_of(".");
					string filename = file.substr(0, tok);
					string checkPath = "../Contents/Material/" + filename;
					//이경로가 있는지 체크
					if (!PathFileExistsA(checkPath.c_str()))
					{
						//없으면 만들어라
						CreateDirectoryA(checkPath.c_str(), NULL);
					}
					//file/name.mtl
					mtlList[name]->SaveFile(filename + "/" + name + ".mtl");
				}
			}
		}
	}
}

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
	cam1->viewport.x = 0.0f;
	cam1->viewport.y = 0.0f;
	cam1->viewport.width = App.GetWidth();
	cam1->viewport.height = App.GetHeight();
	cam1->width = App.GetWidth();
	cam1->height = App.GetHeight();


	root = Actor::Create();
	ifstream fin;
	string file = "box.obj";

	fin.open("../Contents/" + file);
	if (fin.is_open())
	{
		int GCount = 0;
		vector<VertexPTN> Vertices;
		vector<UINT> Indices;
		vector<Vector3> P;
		vector<Vector2> T;
		vector<Vector3> N;
		string name;
		string TexName;
		while (not fin.eof())
		{
			string tag;
			fin >> tag;
			
			if (tag == "mtllib")
			{
				string file;
				fin >> file;
				ReadMtl(file.substr(2, file.size()));
			}
			if (tag == "v")
			{
				Vector3 pos;
				fin >> pos.x >> pos.y >> pos.z;
				P.push_back(pos);
				//cout << pos.x << "," << pos.y << "," <<pos.z << endl;
			}
			else if (tag == "vt")
			{
				Vector2 uv;
				//fin >> uv.x >> uv.y;
				
				float temp;
				fin >> uv.x >> uv.y >> temp;
				uv.y *= -1.0f;
				T.push_back(uv);
			}
			else if (tag == "vn")
			{
				Vector3 normal;
				fin >> normal.x >> normal.y >> normal.z;
				N.push_back(normal);
				//cout << uv.x << "," << uv.y << endl;
			}
			else if (tag == "f")
			{
				int idx[3]; char slash;
				VertexPTN pt;
				for (int i = 0; i < 3; i++)
				{
					fin >> idx[0] >> slash >> idx[1] >> slash >> idx[2];

					//cout << idx[0] << "," << idx[1] << endl;

					pt.position = P[idx[0] - 1];
					pt.uv = T[idx[1] - 1];
					pt.normal = N[idx[2] - 1];
					Indices.push_back(Vertices.size());
					Vertices.push_back(pt);
				}
			}
			else if (tag == "g")
			{
				GCount++;
				if (GCount == 2)
				{
					fin >> name;
				}
				if (GCount == 3)
				{
					GameObject* child = GameObject::Create(name);
					child->mesh =
						make_shared<Mesh>(&Vertices[0], Vertices.size(), Indices.data(), Indices.size(), VertexType::PTN);
					child->shader
						= RESOURCE->shaders.Load("3.Cube.hlsl");
					root->AddChild(child);
					GCount = 1;
					Vertices.clear();
					Indices.clear();
					child->material = mtlList[TexName];
					//child->texture = mapKd[TexName];
					{
						size_t tok = file.find_last_of(".");
						string checkPath = "../Contents/Mesh/" + file.substr(0, tok);
						if (!PathFileExistsA(checkPath.c_str()))
						{
							CreateDirectoryA(checkPath.c_str(), NULL);
						}

						child->mesh->SaveFile(file.substr(0, tok) + "/" + name + ".mesh");
					}
				}
			}
			else if (tag == "usemtl")
			{
				fin >> TexName;
			}
			//한줄 넘기기
			char c[128];
			fin.getline(c, 128);
		}

		/*for (int i = 0; i < Vertices.size(); i++)
		{
			cout << Vertices[i].position.x << ",";
			cout << Vertices[i].position.y << ",";
			cout << Vertices[i].position.z << endl;
		}*/


		//다읽고난 후 메쉬 생성
		
	}

	

}

void Main::Release()
{
}

void Main::Update()

{
	ImGui::Begin("Hierarchy");
	root->RenderHierarchy();
	ImGui::End();

	cam1->ControlMainCam();

	root->Update();
	grid->Update();

	cam1->Update();
}

void Main::LateUpdate()
{
	

}
void Main::PreRender()
{
}

void Main::Render()
{
	cam1->Set();
	LIGHT->Set();
	grid->Render();
	root->Render();
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
#include "stdafx.h"
#include "Scene2.h"


Scene2::Scene2()
{
	grid = Grid::Create();

	cam1 = Camera::Create();
	cam1->LoadFile("Cam2.xml");


	CamCollsion = Actor::Create();
	CamCollsion->LoadFile("Sphere.xml");

	//cam1->AddChild(CamCollsion);
	Camera::main = cam1;

	map = Terrain::Create("ground");
	// map->material->normalMap = RESOURCE->textures.Load("concrete_modular_1x1.png");
	map->material->LoadFile("TestMap1.mtl");
	map->CreateStructuredBuffer();
	//map->TreeCreateIntersect();
	map->garo = 100;
	map->MeshResizeUpdate();

	skybox = Sky::Create();
	skybox->LoadFile("Sky1.xml");
	skybox2 = Sky::Create();
	skybox2->LoadFile("Sky2.xml");

	Tile = Actor::Create();


}

Scene2::~Scene2()
{
}


const int rows = 20;
const int cols = 20;
const int floors = 1;
const int dx[4] = { -1, 1, 0, 0 };
const int dy[4] = { 0, 0, -1, 1 };
int Tiles[rows][cols][floors];
bool visited[rows][cols][floors];

void Scene2::Init()
{
	// �ʱ� �迭 �� ���� (���÷� 0 �Ǵ� 1�� ����)
	// �� �κ��� ���� �����ͷ� �ʱ�ȭ�ؾ� �մϴ�.
	// ���⼭�� ������ ������ ���� 0 �Ǵ� 1�� �ʱ�ȭ�մϴ�.
	unordered_set <double> table;
	double avg = 0.0f;
	float threshold = 10;
	float thres = 0.1;

	//�޸������� �������� ����� -0.3���� �������� ����
	do {
		table.clear();
		siv::PerlinNoise pn(RANDOM->Float(0, 100));
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				for (int k = 0; k < floors; k++) {
					double x = (double)i / thres;
					double y = (double)j / thres;
					double z = (double)k + 0.5;
					double temp = pn.noise3D(x, y, z) * 10;
					table.insert(temp);
					Tiles[i][j][k] = temp > -0.3f ? 1 : 0;
				}
			}
		}
		for (auto it = table.begin(); it != table.end(); it++) {
			avg += *it;
		}
		avg /= table.size();
	} while (avg < -0.3f);

	//����� �������� ���� �ֺ��� Ÿ���� 1�� 7�� �̻��ϰ�� �ڽŵ� 1�� �ٲ��ִ� �ڵ�
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < floors; k++) {
				// 0�� ������ ������ ����
				int count = 0;
				// Ÿ���̶�� �ǳʶڴ�.
				if (not Tiles[i][j][k]) {
					for (int dx = -1; dx <= 1; ++dx) {
						for (int dy = -1; dy <= 1; ++dy) {
							// �ֺ� Ÿ���� ��ġ ���
							int ni = i + dx;
							int nj = j + dy;
							// �迭 ������ ������� Ȯ��
							if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
								// �ֺ� Ÿ���� 0�� ��� count ����
								if (Tiles[ni][nj][k] == 1) {
									count++;
								}
							}
						}
					}
					// ���ǿ� ���� ���� Ÿ�� �� ����
					if (count >= 7) {
						Tiles[i][j][k] = 1;
					}
					else {
						Tiles[i][j][k] = 0;
					}
				}
			}
		}
	}

	bool check = false;
	do {
		cout << "�ٽ�" << endl;
		int size = 0;
		for (int z = 0; z < floors; z++) {
			int x = RANDOM->Int(0, rows);
			int y = RANDOM->Int(0, cols);
			cout << "x , y" << x << ", " << y << endl;
			if (Tiles[x][y][z] == 1 && !visited[x][y][z]) {
				for (int i = 0; i < rows; ++i) {
					for (int j = 0; j < cols; ++j) {
						for (int k = 0; k < floors; ++k) {
							visited[i][j][k] = false;
						}
					}
				}
				std::queue<std::tuple<int, int, int>> q;
				q.push(std::make_tuple(x, y, z));
				size++;
				visited[x][y][z] = true;
				while (!q.empty()) {
					auto current = q.front();
					q.pop();
					int x = std::get<0>(current);
					int y = std::get<1>(current);
					int k = std::get<2>(current);
					for (int l = 0; l < 4; ++l) {
						int ni = x + dx[l];
						int nj = y + dy[l];

						// Check if the adjacent cell is within bounds, unvisited, and part of the area
						if ((ni >= 0 && nj >= 0 && ni < rows && nj < cols) && !visited[ni][nj][k] && Tiles[ni][nj][k] == 1) {
							q.push(std::make_tuple(ni, nj, k));
							visited[ni][nj][k] = true;
							Tiles[ni][nj][k] = 2;  // You can mark as visited if needed
							size++;

						}
					}
				}
				cout << size << endl;
			}
		}
		if (size > rows * cols / 3) {
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					for (int k = 0; k < floors; k++) {
						if (Tiles[i][j][k] == 2) {
							check = true;
						}
					}
				}
			}
		}
	} while (!check);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < floors; k++) {
				if (Tiles[i][j][k] == 1) {
					Tiles[i][j][k] = 0;
				}
				else if (Tiles[i][j][k] == 2) {
					Tiles[i][j][k] = 1;
				}
			}
		}
	}



	float gridScale = static_cast<float>(map->garo) / rows;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < floors; k++) {
				if (Tiles[i][j][k] == 1) {
					Actor* temp = Actor::Create();
					temp->LoadFile("Tile1.xml");
					temp->name = to_string(i) + "x" + to_string(j) + "x" + to_string(k);
					//float gridScale = rows / static_cast<float>(map->garo);  // ������ �κ�
					// ������ �κ�: �� Ÿ���� �߽��� ����Ͽ� ��ġ ����
					float centerX = -map->garo / 2.0f + (i * gridScale);
					float centerZ = map->garo / 2.0f - (j * gridScale);
					temp->SetWorldPosX(centerX);
					temp->SetWorldPosZ(centerZ);
					temp->SetWorldPosY(floors * k);
					temp->scale.x;
					temp->scale.z;
					//temp->scale.x *= gridScale;
					//temp->scale.z *= gridScale;
					Tile->AddChild(temp);
				}
			}
		}
	}

	// �� Ÿ�� �ֺ��� Ư�� ������ ������ �� �� ���� (rows, cols ������ ����)
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < floors; k++) {
				// ���� Ÿ���� 1�� ��쿡�� �˻�
				if (Tiles[i][j][k] == 1) {
					// ����� Ž���Ͽ� Ư�� ������ �����ϸ� �� ����
					// �����¿� ���⸸ �˻�
					int dx[4] = { -1, 1, 0, 0 };
					int dy[4] = { 0, 0, -1, 1 };
					for (int l = 0; l < 4; ++l) {
						int ni = i + dx[l];
						int nj = j + dy[l];
						// ������ ����� ���� Ÿ�Ͽ����� ���� �׸�
						Actor* temp = Actor::Create();
						temp->LoadFile("Wall1.xml");

						if (ni < 0 || nj < 0 || ni >= rows || nj >= cols) {
							// ȸ�� ���� �迭
							float rotationAngles[4] = { -90, 90, 0, 180 };
							// ���⿡ ���� ���� ���ִ� �ڵ�
							temp->rotation.y = rotationAngles[l] * ToRadian;
							if (l == 0) {
								temp->SetLocalPosZ(-5);
							}
							else if (l == 1) {
								temp->SetLocalPosX(5);
							}
							else if (l == 3) {
								temp->SetLocalPosX(5);
								temp->SetLocalPosZ(-5);
							}
							temp->name = "floor" + to_string(k) + "_" + to_string(i) + "y" + to_string(j) + "_" + to_string(l);
							Tile->Find(to_string(i) + "x" + to_string(j) + "x" + to_string(k))->AddChild(temp);
						}
						else if (Tiles[ni][nj][k] != 1) {
							// ���� �׸��� ����
							// ȸ�� ���� �迭
							float rotationAngles[4] = { -90, 90, 0, 180 };
							// ���⿡ ���� ���� ���ִ� �ڵ�
							temp->rotation.y = rotationAngles[l] * ToRadian;
							if (l == 0) {
								temp->SetLocalPosZ(-5);
							}
							else if (l == 1) {
								temp->SetLocalPosX(5);
							}
							else if (l == 3) {
								temp->SetLocalPosX(5);
								temp->SetLocalPosZ(-5);
							}
							temp->name = "floor" + to_string(k) + "_" + to_string(i) + "y" + to_string(j) + "_" + to_string(l);
							Tile->Find(to_string(i) + "x" + to_string(j) + "x" + to_string(k))->AddChild(temp);
						}
					}
				}
			}
		}
	}
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
	CamCollsion->RenderHierarchy();
	Tile->RenderHierarchy();
	ImGui::End();

	if (INPUT->KeyDown(VK_F5)) {
		Tile->ReleaseMember();
		Init();
	}

	cam1->ControlMainCam();

	grid->Update();
	map->Update();
	cam1->Update();

	skybox->Update();
	skybox2->Update();

	Tile->Update();
	CamCollsion->Update();

}

void Scene2::LateUpdate()
{
}

void Scene2::PreRender()
{
	LIGHT->Set();
	Camera::main->Set();

	skybox->Render();
	skybox2->Render();

}

void Scene2::Render()
{
	LIGHT->Set();
	cam1->Set();

	skybox->Render();
	skybox2->Render();

	CamCollsion->Render();
	map->Render();
	Tile->Render();
}

void Scene2::ResizeScreen()
{
}

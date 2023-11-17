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
	// 초기 배열 값 설정 (예시로 0 또는 1로 설정)
	// 이 부분을 실제 데이터로 초기화해야 합니다.
	// 여기서는 간단한 예제를 위해 0 또는 1로 초기화합니다.
	unordered_set <double> table;
	double avg = 0.0f;
	float threshold = 10;
	float thres = 0.1;

	//펄린노이즈 난수값의 평균이 -0.3보다 작을때만 실행
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

	//빈공간 기준으로 만약 주변에 타일이 1이 7개 이상일경우 자신도 1로 바꿔주는 코드
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < floors; k++) {
				// 0의 개수를 저장할 변수
				int count = 0;
				// 타일이라면 건너뛴다.
				if (not Tiles[i][j][k]) {
					for (int dx = -1; dx <= 1; ++dx) {
						for (int dy = -1; dy <= 1; ++dy) {
							// 주변 타일의 위치 계산
							int ni = i + dx;
							int nj = j + dy;
							// 배열 범위를 벗어나는지 확인
							if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
								// 주변 타일이 0인 경우 count 증가
								if (Tiles[ni][nj][k] == 1) {
									count++;
								}
							}
						}
					}
					// 조건에 따라 현재 타일 값 변경
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
		cout << "다시" << endl;
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
					//float gridScale = rows / static_cast<float>(map->garo);  // 수정된 부분
					// 수정된 부분: 각 타일의 중심을 계산하여 위치 설정
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

	// 각 타일 주변에 특정 조건을 만족할 때 벽 생성 (rows, cols 사이즈 기준)
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < floors; k++) {
				// 현재 타일이 1인 경우에만 검사
				if (Tiles[i][j][k] == 1) {
					// 사방을 탐색하여 특정 조건을 만족하면 벽 생성
					// 상하좌우 방향만 검사
					int dx[4] = { -1, 1, 0, 0 };
					int dy[4] = { 0, 0, -1, 1 };
					for (int l = 0; l < 4; ++l) {
						int ni = i + dx[l];
						int nj = j + dy[l];
						// 범위를 벗어나면 현재 타일에서도 벽을 그림
						Actor* temp = Actor::Create();
						temp->LoadFile("Wall1.xml");

						if (ni < 0 || nj < 0 || ni >= rows || nj >= cols) {
							// 회전 각도 배열
							float rotationAngles[4] = { -90, 90, 0, 180 };
							// 방향에 따른 생성 해주는 코드
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
							// 벽을 그리는 조건
							// 회전 각도 배열
							float rotationAngles[4] = { -90, 90, 0, 180 };
							// 방향에 따른 생성 해주는 코드
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

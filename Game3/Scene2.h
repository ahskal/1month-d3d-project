#pragma once

struct MapGenerator {

	const int dx[4] = { -1, 1, 0, 0 };
	const int dy[4] = { 0, 0, -1, 1 };

	int rows, cols, floors;
	std::vector<std::vector<std::vector<int>>> Tiles;
	std::vector<std::vector<std::vector<bool>>> visited;


	// Constructor
	MapGenerator(int rows, int cols, int floors) : rows(rows), cols(cols), floors(floors) {
		Tiles.resize(rows, std::vector<std::vector<int>>(cols, std::vector<int>(floors, 0)));
		visited.resize(rows, std::vector<std::vector<bool>>(cols, std::vector<bool>(floors, false)));
	}
	// 초기 생성
	void generateInitialMap() {
		unordered_set <double> table;
		double avg = 0.0f;
		float threshold = 10;
		float thres = 0.1;

		//펄린노이즈 난수값의 평균이 -0.3보다 작을때만 실행
		do {
			table.clear();
			siv::PerlinNoise pn(RANDOM->Float(0, 100));
			for (int k = 0; k < floors; k++) {
				for (int i = 0; i < rows; i++) {
					for (int j = 0; j < cols; j++) {
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
	}

	// 생성 예외처리

	//void updateTiles() {
	//	//빈공간 기준으로 만약 주변에 타일이 1이 7개 이상일경우 자신도 1로 바꿔주는 코드
	//	for (int i = 0; i < rows; i++) {
	//		for (int j = 0; j < cols; j++) {
	//			for (int k = 0; k < floors; k++) {
	//				// 0의 개수를 저장할 변수
	//				int count = 0;
	//				// 타일이라면 건너뛴다.
	//				if (not Tiles[i][j][k]) {
	//					for (int dx = -1; dx <= 1; ++dx) {
	//						for (int dy = -1; dy <= 1; ++dy) {
	//							// 주변 타일의 위치 계산
	//							int ni = i + dx;
	//							int nj = j + dy;
	//							// 배열 범위를 벗어나는지 확인
	//							if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
	//								// 주변 타일이 0인 경우 count 증가
	//								if (Tiles[ni][nj][k] == 1) {
	//									count++;
	//								}
	//							}
	//						}
	//					}
	//					// 조건에 따라 현재 타일 값 변경
	//					if (count >= 7) {
	//						Tiles[i][j][k] = 1;
	//					}
	//					else {
	//						Tiles[i][j][k] = 0;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	// 탐색
	vector<std::tuple<int, int, int>> bfs(int startX, int startY, int startFloor) {
		std::vector<std::tuple<int, int, int>> BigTiles;

		for (int k = 0; k < floors; k++) {
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					visited[i][j][k] = false;
				}
			}
		}
		std::queue<std::tuple<int, int, int>> q;
		q.push(std::make_tuple(startX, startY, startFloor));
		auto current = q.front();
		BigTiles.push_back(q.front());

		visited[startX][startY][startFloor] = true;

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
					BigTiles.push_back(std::make_tuple(ni, nj, k));
					visited[ni][nj][k] = true;
					// grid[ni][nj][k] = 2;  // You can mark as visited if needed
				}
			}
		}
		return BigTiles;
	}

	// 연결성 체크
	void checkConnectivity() {
		for (int k = 0; k < floors; k++) {
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					visited[i][j][k] = false;
				}
			}
		}

		std::vector<std::tuple<int, int, int>> bigTiles;  // 큰 타일의 좌표를 저장하는 벡터		

		for (int k = 0; k < floors; k++) {
			int max_size = 0;
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					if (Tiles[i][j][k] == 1 && !visited[i][j][k]) {
						auto temp = bfs(i, j, k);
						if (temp.size() > bigTiles.size()) {
							bigTiles.clear();
							bigTiles = temp;
							max_size = bigTiles.size();
						}
					}
				}
			}
			if ((float)max_size > (float)(rows * cols) / 2) {
				for (int i = 0; i < rows; i++) {
					for (int j = 0; j < cols; j++) {
						Tiles[i][j][k] = 0;
					}
				}
				for (const auto& tile : bigTiles) {
					int x = std::get<0>(tile);
					int y = std::get<1>(tile);
					int z = std::get<2>(tile);
					// 필요한 처리를 수행
					Tiles[x][y][z] = 1;  // 예시: 2로 표시하는 동작
				}
			}
		}


	}

	void printConnectedComponentsSize() {
		// ... (code for printing connected components size)
	}

	// 마지막 생성
	void finalizeMap(GameObject* act) {
		float tileSize = 5.0f;
		float tileWidth = tileSize * rows;
		float tileHeight = tileSize * cols;
		float centerX = -tileWidth / 2;
		float centerZ = -tileHeight / 2;
		for (int k = 0; k < floors; k++) {
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					if (Tiles[i][j][k] == 1) {
						Actor* temp = Actor::Create();

						temp->LoadFile("Tile1.xml");
						temp->name = to_string(i) + "x" + to_string(j) + "x" + to_string(k);
						temp->SetLocalPosX(centerX + (tileSize * i));
						temp->SetLocalPosZ(centerZ + (tileSize * j));
						temp->SetWorldPosY(k * tileSize);
						temp->scale.x;
						temp->scale.z;
						act->AddChild(temp);
					}
				}
			}
		}
	}

	void WallCreateMap(Actor* act) {
		// 각 타일 주변에 특정 조건을 만족할 때 벽 생성 (rows, cols 사이즈 기준)
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				for (int k = 0; k < floors; k++) {
					// 현재 타일이 1인 경우에만 검사
					if (Tiles[i][j][k] == 1) {
						// 사방을 탐색하여 특정 조건을 만족하면 벽 생성
						// 상하좌우 방향만 검사
						for (int l = 0; l < 4; ++l) {
							int ni = i + dx[l];
							int nj = j + dy[l];
							// 범위를 벗어나면 현재 타일에서도 벽을 그림
							Actor* temp = Actor::Create();
							temp->LoadFile("Wall1.xml");
							// 회전 각도 배열
							float rotationAngles[4] = { 0, -180, -90, 90 };
							// 방향에 따른 생성 해주는 코드
							temp->rotation.y = rotationAngles[l] * ToRadian;
							if (ni < 0 || nj < 0 || ni >= rows || nj >= cols) {
								if (l == 1) {
									temp->SetLocalPosX(5);
									temp->SetLocalPosZ(-5);
								}
								else if (l == 2) {
									temp->SetLocalPosZ(-5);
								}
								else if (l == 3) {
									temp->SetLocalPosX(5);
								}
								temp->name = "floor" + to_string(k) + "_" + to_string(i) + "y" + to_string(j) + "_" + to_string(l);
								act->Find(to_string(i) + "x" + to_string(j) + "x" + to_string(k))->AddChild(temp);
							}
							else if (Tiles[ni][nj][k] != 1) {
								if (l == 1) {
									temp->SetLocalPosX(5);
									temp->SetLocalPosZ(-5);
								}
								else if (l == 2) {
									temp->SetLocalPosZ(-5);
								}
								else if (l == 3) {
									temp->SetLocalPosX(5);
								}
								temp->name = "floor" + to_string(k) + "_" + to_string(i) + "y" + to_string(j) + "_" + to_string(l);
								act->Find(to_string(i) + "x" + to_string(j) + "x" + to_string(k))->AddChild(temp);
							}
						}
					}
				}
			}
		}
	}
};

class Scene2 : public Scene
{
private:
	Camera* cam1;
	Actor* CamCollsion;

	Grid* grid;
	Terrain* map;
	Sky* skybox;
	Sky* skybox2;

	Actor* Tile;

public:
	Scene2();
	~Scene2();

	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};

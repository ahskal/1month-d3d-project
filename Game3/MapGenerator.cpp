#include "stdafx.h"
#include "MapGenerator.h"

void MapGenerator::set(int rows, int cols, int floors)
{
	this->rows = rows;
	this->cols = cols;
	this->floors = floors;
	Tiles.resize(rows, std::vector<std::vector<int>>(cols, std::vector<int>(floors, 0)));
	visited.resize(rows, std::vector<std::vector<bool>>(cols, std::vector<bool>(floors, false)));
}

void MapGenerator::generateInitialMap()
{
	unordered_set <double> table;
	double avg = 0.0f;
	float threshold = 10;
	float thres = 0.1;

	//�޸������� �������� ����� -0.3���� �������� ����
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

void MapGenerator::updateTiles()
{
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
}

vector<std::tuple<int, int, int>> MapGenerator::bfs(int startX, int startY, int startFloor)
{
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

void MapGenerator::checkConnectivity()
{
	for (int k = 0; k < floors; k++) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				visited[i][j][k] = false;
			}
		}
	}

	std::vector<std::tuple<int, int, int>> bigTiles;  // ū Ÿ���� ��ǥ�� �����ϴ� ����		

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
					if (max_size > (rows * cols) / 2) {
						break;
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
				// �ʿ��� ó���� ����
				Tiles[x][y][z] = 1;  // ����: 2�� ǥ���ϴ� ����
			}
		}
	}
}

void MapGenerator::coutTile()
{
	for (int k = 0; k < floors; k++) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				string temp = Tiles[i][j][k] == 1 ? "��" : "��";
				cout << temp << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
}

void MapGenerator::InstanceTile(GameObject* act)
{
	float tileSize = 5.0f;
	float tileWidth = tileSize * rows;
	float tileHeight = tileSize * cols;
	float centerX = -tileWidth / 2;
	float centerZ = -tileHeight / 2;

	float IndexWidth = rows * cols * floors;
	Matrix* matrix = new Matrix[IndexWidth];

	Actor* temp = Actor::Create();
	temp->LoadFile("Tile1.xml");
	temp->name = "Floor_Instance";
	temp->shader = RESOURCE->shaders.Load("4.Instance_Deferred.hlsl");
	int count2 = 0;
	for (int k = 0; k < floors; k++) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				Matrix temp = Matrix::CreateTranslation(Vector3(centerX + i * 5, k * 5, centerZ + j * 5));
				matrix[count2] = temp;
				matrix[count2] = temp.Transpose();
				count2++;
			}
		}
	}
	temp->mesh->CreateInstanceBuffer(matrix, count2);
	act->AddChild(temp);
}

void MapGenerator::finalizeMap(GameObject* act)
{

	float tileWidth = tileSize * rows;
	float tileHeight = tileSize * cols;
	float centerX = -tileWidth / 2;
	float centerZ = -tileHeight / 2;

	for (int k = 0; k < floors; k++) {
		Actor* floor = Actor::Create();
		floor->name = "floor" + to_string(k);
		act->AddChild(floor);
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (Tiles[i][j][k] == 1) {
					Actor* temp = Actor::Create();
					//temp->LoadFile("Tile1.xml");
					temp->name = to_string(i) + "x" + to_string(j) + "x" + to_string(k);
					temp->SetLocalPosX(centerX + (tileSize * i));
					temp->SetLocalPosZ(centerZ + (tileSize * j));
					temp->SetWorldPosY(k * tileSize);
					temp->scale.x;
					temp->scale.z;
					floor->AddChild(temp);
				}
			}
		}
	}
}

void MapGenerator::WallCreateMap(Actor* act)
{
	// �� Ÿ�� �ֺ��� Ư�� ������ ������ �� �� ���� (rows, cols ������ ����)
	for (int k = 0; k < floors; k++) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				// ���� Ÿ���� 1�� ��쿡�� �˻�
				if (Tiles[i][j][k] == 1) {
					// ����� Ž���Ͽ� Ư�� ������ �����ϸ� �� ����
					// �����¿� ���⸸ �˻�
					for (int l = 0; l < 4; ++l) {
						int ni = i + dx[l];
						int nj = j + dy[l];
						// ������ ����� ���� Ÿ�Ͽ����� ���� �׸�
						Actor* temp = Actor::Create();
						temp->LoadFile("Wall1.xml");
						temp->shader = RESOURCE->shaders.Load("4.Cube_Deferred.hlsl");
						// ȸ�� ���� �迭
						float rotationAngles[4] = { -90, 90, -180, 0 };
						// ���⿡ ���� ���� ���ִ� �ڵ�
						temp->rotation.y = rotationAngles[l] * ToRadian;
						if (ni < 0 || nj < 0 || ni >= rows || nj >= cols) {
							if (l == 1) {
								temp->SetLocalPosX(5);
								temp->SetLocalPosZ(5);
							}
							else if (l == 2) {
								temp->SetLocalPosX(5);
							}
							else if (l == 3) {
								temp->SetLocalPosZ(5);
							}
							temp->name = "floor" + to_string(k) + "_" + to_string(i) + "y" + to_string(j) + "_" + to_string(l);
							act->Find(to_string(i) + "x" + to_string(j) + "x" + to_string(k))->AddChild(temp);
							WallActorList.push_back(temp);
						}
						else if (Tiles[ni][nj][k] != 1) {
							if (l == 1) {
								temp->SetLocalPosX(5);
								temp->SetLocalPosZ(5);
							}
							else if (l == 2) {
								temp->SetLocalPosX(5);
							}
							else if (l == 3) {
								temp->SetLocalPosZ(5);
							}
							temp->name = "floor" + to_string(k) + "_" + to_string(i) + "y" + to_string(j) + "_" + to_string(l);
							act->Find(to_string(i) + "x" + to_string(j) + "x" + to_string(k))->AddChild(temp);
							WallActorList.push_back(temp);
						}

					}
				}
			}
		}
	}
}

bool MapGenerator::WorldPosToTileIdx(Vector3 WPos, Int2& TileIdx)
{
	float tileWidth = tileSize * rows;
	float tileHeight = tileSize * cols;

	// �߽��� ��� �� �̵�
	Vector3 centerOffset(tileWidth / 2, 0, tileHeight / 2);
	WPos += centerOffset;

	// Ÿ�� ��ǥ ���
	Vector2 tileCoord;
	tileCoord.x = WPos.x / tileSize;
	tileCoord.y = WPos.z / tileSize;

	if ((tileCoord.x < 0) or (tileCoord.y < 0) or
		(tileCoord.x >= rows) or (tileCoord.y >= cols))
	{
		return false;
	}

	// ��� ����
	TileIdx.x = static_cast<int>(tileCoord.x);
	TileIdx.y = static_cast<int>(tileCoord.y);

	return true;
}

int MapGenerator::GetTileState(Vector3 WPos)
{
	Int2 idx;
	int floor = WPos.y / 5.0f;
	if (WorldPosToTileIdx(WPos, idx)) {
		return GetTileState(idx, floor);
	}
	return false;
}

bool MapGenerator::GetTileState(Int2 TileIdx, int floor)
{
	return Tiles[TileIdx.x][TileIdx.y][floor];
}

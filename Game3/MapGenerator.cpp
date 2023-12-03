#include "stdafx.h"
#include "MapGenerator.h"

void MapGenerator::initializeMap()
{
	unordered_set<double>	noiseValues;
	double					averageNoise = 0.0;
	const double			noiseThreshold = -0.3;
	const float				noiseResolution = 0.1;
	// Generate map until the average noise is below the threshold
	do
	{
		noiseValues.clear();
		siv::PerlinNoise perlinNoise(RANDOM->Float(0, 10000));
		for (int rowIndex = 0; rowIndex < rows; rowIndex++)
		{
			for (int colIndex = 0; colIndex < cols; colIndex++)
			{
				double x = static_cast<double>(rowIndex) / noiseResolution;
				double y = static_cast<double>(colIndex) / noiseResolution;
				double z = 0.5f;
				double noiseValue = perlinNoise.noise3D(x, y, z) * 10;

				noiseValues.insert(noiseValue);
				Tiles[rowIndex][colIndex] = (noiseValue > noiseThreshold) ? 1 : 0;
			}
		}
		// Calculate average noise
		for (auto it = noiseValues.begin(); it != noiseValues.end(); it++)
		{
			averageNoise += *it;
		}
		averageNoise /= noiseValues.size();
	} while (averageNoise < noiseThreshold);

	// 가장자리 처리
	for (int i = 0; i < rows; i++) {
		Tiles[i][0] = 0;          // 왼쪽 가장자리
		Tiles[i][cols - 1] = 0;   // 오른쪽 가장자리
	}

	for (int j = 0; j < cols; j++) {
		Tiles[0][j] = 0;          // 위쪽 가장자리
		Tiles[rows - 1][j] = 0;   // 아래쪽 가장자리
	}
}

void MapGenerator::updateMapTiles()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			int count = 0;
			if (not Tiles[i][j])
			{
				for (int dx = -1; dx <= 1; ++dx)
				{
					for (int dy = -1; dy <= 1; ++dy)
					{
						// 주변 타일의 위치 계산
						int ni = i + dx;
						int nj = j + dy;
						// 배열 범위를 벗어나는지 확인
						if (ni >= 0 && ni < rows && nj >= 0 && nj < cols)
						{
							// 주변 타일이 0인 경우 count 증가
							if (Tiles[ni][nj] == 1)
							{
								count++;
							}
						}
					}
				}
			}
			if (count >= 7) {
				Tiles[i][j] = 1;
			}
			else {
				Tiles[i][j] = 0;
			}
		}
	}
}

void MapGenerator::validateMapConnectivity()
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			visited[i][j] = false;
		}
	}
	const int minRequiredSize = (float)(rows * cols) / 2;
	std::vector<std::pair<int, int>> bigTiles;
	bigTiles.clear();

	// 큰 타일의 좌표를 저장하는 벡터	
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (Tiles[i][j] == 1 && !visited[i][j]) {
				auto temp = bfs(i, j);
				if (temp.size() > bigTiles.size()) {
					bigTiles.clear();
					bigTiles = temp;
				}
			}
		}
	}
	for (int x = 0; x < rows; x++) {
		for (int y = 0; y < cols; y++) {
			Tiles[x][y] = 0;  // 모든 타일을 0으로 초기화
		}
	}
	for (const auto& tile : bigTiles) {
		int x = std::get<0>(tile);
		int y = std::get<1>(tile);
		// 필요한 처리를 수행
		Tiles[x][y] = 1;  // 예시: 2로 표시하는 동작
	}
}

vector<std::pair<int, int>> MapGenerator::bfs(int startX, int startY)
{
	vector<std::pair<int, int>> BigTiles;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			visited[i][j] = false;
		}
	}
	std::queue<std::pair<int, int>> q;
	q.push(std::make_pair(startX, startY));
	auto current = q.front();
	BigTiles.push_back(q.front());
	visited[startX][startY] = true;
	while (!q.empty())
	{
		auto current = q.front();
		q.pop();

		int x = std::get<0>(current);
		int y = std::get<1>(current);

		for (int l = 0; l < 4; ++l)
		{
			int ni = x + dx[l];
			int nj = y + dy[l];

			// Check if the adjacent cell is within bounds, unvisited, and part of the area
			if ((ni >= 0 && nj >= 0 && ni < rows && nj < cols) && !visited[ni][nj] && Tiles[ni][nj] == 1)
			{
				q.push(std::make_pair(ni, nj));
				BigTiles.push_back(std::make_pair(ni, nj));
				visited[ni][nj] = true;
			}
		}
	}
	return BigTiles;
}

void MapGenerator::printTileInfo()
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			string temp = Tiles[i][j] == 1 ? "■" : "□";
			cout << temp << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void MapGenerator::instantiateTile(GameObject* act)
{
	float tileSize = 5.0f;
	float tileWidth = tileSize * rows;
	float tileHeight = tileSize * cols;
	float centerX = -tileWidth / 2;
	float centerZ = -tileHeight / 2;
	float IndexWidth = rows * cols * 2;
	Matrix* matrix = new Matrix[IndexWidth];
	Actor* temp = Actor::Create();
	temp->LoadFile("Object/Tile1.xml");
	temp->name = "Floor_Instance";
	temp->shader = RESOURCE->shaders.Load("4.Instance_Deferred.hlsl");
	int count2 = 0;
	for (int k = 0; k < 2; k++) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				Matrix temp = Matrix::CreateTranslation(Vector3(centerX + i * 5, k * 5, centerZ + j * 5 + 5));
				//matrix[count2] = temp;
				matrix[count2] = temp.Transpose(); // 왜 여기서 두 번 할당하는지 확인 필요
				count2++;
			}
		}
	}

	// matrix 배열을 메모리에 할당한 후 해제
	temp->mesh->CreateInstanceBuffer(matrix, count2);
	delete[] matrix;

	act->AddChild(temp);
}

void MapGenerator::finalizeMapConfiguration(GameObject* act)
{
	float tileWidth = tileSize * rows;
	float tileHeight = tileSize * cols;
	float centerX = -tileWidth / 2;
	float centerZ = -tileHeight / 2;

	Actor* Tile = Actor::Create();
	Tile->name = "Tile";
	act->AddChild(Tile);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (Tiles[i][j] == 0) {
				Actor* temp = Actor::Create();
				//temp->LoadFile("Object/Cube.xml");
				temp->collider = new Collider(ColliderType::OBOX);
				temp->collider->scale = Vector3(2.5f, 2.5f, 2.5f);
				temp->collider->SetLocalPosY(2.5f);
				temp->name = to_string(i) + "x" + to_string(j);
				temp->SetLocalPosX(centerX + (tileSize * i) + 2.5f);
				temp->SetLocalPosZ(centerZ + (tileSize * j) + 2.5f);
				temp->scale.x;
				temp->scale.z;
				Tile->AddChild(temp);
				WallActorList.push_back(temp);
			}
		}
	}
}

void MapGenerator::createWallMap(Actor* act)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			auto actor = act->Find(to_string(i) + "x" + to_string(j));
			if (Tiles[i][j] == 0)
			{
				for (int l = 0; l < 4; ++l)
				{
					int ni = i + dx[l];
					int nj = j + dy[l];
					Actor* temp = Actor::Create();
					temp->LoadFile("Object/Wall1.xml");
					temp->shader = RESOURCE->shaders.Load("4.Cube_Deferred.hlsl");
					// 방향에 따른 생성 해주는 코드
					if (ni < 0 || nj < 0 || ni >= rows || nj >= cols)
					{
						float rotationAngles[4] = { -90, 90, -180, 0 };
						temp->rotation.y = rotationAngles[l] * ToRadian;
						if (l == 0)	temp->SetLocalPosX(-2.5);
						else if (l == 1) temp->SetLocalPosX(2.5);
						else if (l == 2) temp->SetLocalPosZ(-2.5);
						else if (l == 3) temp->SetLocalPosZ(2.5);
						temp->name = to_string(i) + "y" + to_string(j) + "_" + to_string(l);
						actor->AddChild(temp);
					}
					else if (Tiles[ni][nj] != 0)
					{
						float rotationAngles[4] = { 90, -90, 0, -180 };
						temp->rotation.y = rotationAngles[l] * ToRadian;
						if (l == 0)	temp->SetLocalPosX(-2.5);
						else if (l == 1) temp->SetLocalPosX(2.5);
						else if (l == 2) temp->SetLocalPosZ(-2.5);
						else if (l == 3) temp->SetLocalPosZ(2.5);
						temp->name = to_string(i) + "y" + to_string(j) + "_" + to_string(l);
						actor->AddChild(temp);
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

	// 중심점 계산 및 이동
	Vector3 centerOffset(tileWidth / 2, 0, tileHeight / 2);
	WPos += centerOffset;

	// 타일 좌표 계산
	Vector2 tileCoord;
	tileCoord.x = WPos.x / tileSize;
	tileCoord.y = WPos.z / tileSize;

	if ((tileCoord.x < 0) or (tileCoord.y < 0) or
		(tileCoord.x >= rows) or (tileCoord.y >= cols))
	{
		return false;
	}

	// 결과 저장
	TileIdx.x = static_cast<int>(tileCoord.x);
	TileIdx.y = static_cast<int>(tileCoord.y);

	return true;
}

int MapGenerator::GetTileState(Vector3 WPos)
{
	Int2 idx;
	if (WorldPosToTileIdx(WPos, idx)) {
		return GetTileState(idx);
	}
	return false;
}

bool MapGenerator::GetTileState(Int2 TileIdx)
{
	return Tiles[TileIdx.x][TileIdx.y];
}

void MapGenerator::createLighting(Actor* act)
{
	int count = 0;
	const int Range = 5;

	for (int i = 0; i < Range; i++) {
		for (int j = 0; j < Range; j++) {
			Light* light = Light::Create("Light" + std::to_string(count), 1);
			light->rotation.x = 45 * ToRadian;
			light->light->radius = 15;
			light->light->outer = 90;
			light->light->inner = 0;
			// Calculate world position based on tile coordinates
			float posX = ((i * 4) - (rows - 4) / 2) * tileSize;
			float posY = 5;  // Assuming a fixed Y position
			float posZ = ((j * 4) - (rows - 4) / 2) * tileSize;
			Vector3 pos = Vector3(posX, posY, posZ);

			light->SetWorldPos(pos);
			act->AddChild(light);
			count++;
		}
	}
}
bool MapGenerator::GetRandomPos(Int2 TileIdx)
{
	return Tiles[TileIdx.x][TileIdx.y] == 1;
}
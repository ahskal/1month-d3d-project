#pragma once
class MapGenerator
{
private:
	const int dx[4] = { -1, 1, 0, 0 };
	const int dy[4] = { 0, 0, -1, 1 };

public:
	int rows, cols, floors;
	float tileSize = 5.0f;
	vector<vector<vector<int>>> Tiles;
	vector<vector<vector<bool>>> visited;
	Actor* target;


	vector<Actor*> WallActorList;
	MapGenerator() : rows(0), cols(0), floors(0) {}
	MapGenerator(int rows, int cols, int floors) : rows(rows), cols(cols), floors(floors) {
		Tiles.resize(rows, std::vector<std::vector<int>>(cols, std::vector<int>(floors, 0)));
		visited.resize(rows, std::vector<std::vector<bool>>(cols, std::vector<bool>(floors, false)));
		WallActorList.resize(0);
		target = nullptr;
	}
	void set(int rows, int cols, int floors);
	void generateInitialMap();

	void updateTiles();
	
	vector<std::tuple<int, int, int>> bfs(int startX, int startY, int startFloor);
	void checkConnectivity();
	void coutTile();
	void InstanceTile(GameObject* act);
	void finalizeMap(GameObject* act);
	void WallCreateMap(Actor* act);
	bool WorldPosToTileIdx(Vector3 WPos, Int2& TileIdx);
	int GetTileState(Vector3 WPos);
	bool GetTileState(Int2 TileIdx, int floor);
};


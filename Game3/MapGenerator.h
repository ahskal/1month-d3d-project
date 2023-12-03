#pragma once
#pragma once
class MapGenerator
{
private:
	const int dx[4] = { -1, 1, 0, 0 };
	const int dy[4] = { 0, 0, -1, 1 };

public:
	int rows, cols, floors;
	float tileSize = 5.0f;
	vector<vector<int>> Tiles;
	vector<vector<bool>> visited;
	vector<Actor*> WallActorList;

	MapGenerator() : rows(1), cols(1), floors(1) {}
	MapGenerator(int rows, int cols, int floors) : rows(rows), cols(cols), floors(floors) {
		Tiles.resize(rows, std::vector<int>(cols, 0));
		visited.resize(rows, std::vector<bool>(cols, false));
		WallActorList.resize(0);
	}
	void initializeMap();
	void updateMapTiles();
	void validateMapConnectivity();
	vector<std::pair<int, int>> bfs(int startX, int startY);
	void printTileInfo();

	void instantiateTile(GameObject* act);
	void finalizeMapConfiguration(GameObject* act);
	void createWallMap(Actor* act);
	void createLighting(Actor* act);

	Vector3 TileRandomPos();

	void createPotal(Actor* act);

	bool WorldPosToTileIdx(Vector3 WPos, Int2& TileIdx);
	int GetTileState(Vector3 WPos);
	bool GetTileState(Int2 TileIdx);


};


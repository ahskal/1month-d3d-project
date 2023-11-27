#pragma once

class Scene2 : public Scene
{
private:
	Camera* cam1;

	class Player* player;
	class MapGenerator *mapGen;

	Grid* grid;
	Terrain* map;
	Sky* skybox;
	Sky* skybox2;

	Light* Lightting;

	Actor* Tile;

	Deferred* deferred;   //4개의 렌더타겟  
	UI* post;       //4개의 렌더타겟을 받아 그려줄

	Rain* rain;

	UI* ui;


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

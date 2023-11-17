#pragma once
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

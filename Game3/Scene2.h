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

	Deferred* deferred;   //4���� ����Ÿ��  
	UI* post;       //4���� ����Ÿ���� �޾� �׷���

	Rain* rain;

	UI* ui;


public:
	Scene2();
	~Scene2();

	virtual void Init() override;
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};

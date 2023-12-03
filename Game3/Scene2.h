#pragma once

class Scene2 : public Scene
{
private:
	Camera* cam1;

	class PlayerData* player;
	class MapGenerator *mapGen;

	Sky* skybox;
	Sky* skybox2;

	Actor* Tile;

	Deferred* deferred;   //4���� ����Ÿ��  
	UI* post;       //4���� ����Ÿ���� �޾� �׷���

	Actor* act;
	
	thread* t1;

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

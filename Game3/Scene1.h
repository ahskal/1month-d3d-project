#pragma once
class Scene1 : public Scene
{
private:
	Camera* cam1;
	Grid* grid;

	Terrain* map;
	Terrain* water;

	Sky* skybox;
	Sky* skybox2;

	float timer;

	Deferred* deferred;   //4���� ����Ÿ��  
	UI* post;       //4���� ����Ÿ���� �޾� �׷���

public:
	Scene1();
	~Scene1();

	virtual void Init() override;
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};

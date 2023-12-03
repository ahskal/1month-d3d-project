#pragma once
class Scene1 : public Scene
{
private:
	Camera* cam1;
	Terrain* map;
	Terrain* water;

	Sky* skybox;

	float timer;

	Deferred* deferred;

	Actor* Build;
	Actor* house;

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

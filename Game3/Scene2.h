#pragma once

class Scene2 : public Scene
{
private:
	Deferred* deferred;   //4���� ����Ÿ��  
	class MapGenerator *mapGen;
	Actor* Tile;
	Camera* cam1;
	Actor* act;

	Light* DarkLight;
	

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

#pragma once

class Scene2 : public Scene
{
private:
	Deferred* deferred;   //4개의 렌더타겟  
	class MapGenerator *mapGen;
	Actor* Tile;
	Camera* cam1;

	UI* post;       //4개의 렌더타겟을 받아 그려줄

	Actor* act;


	

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

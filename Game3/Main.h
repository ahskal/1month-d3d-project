#pragma once

class Main : public Scene
{
private:
	Camera* cam1;
	Grid* grid;
	
	Terrain* map;
	Player* player;
	Sky* skybox;
	Sky* skybox2;



public:
	Main();
	~Main();

	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};

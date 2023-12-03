#pragma once

class Scene3 : public Scene
{
private:
	//Camera* cam1;
	//Grid* grid;
	
	Deferred* deferred;   //4개의 렌더타겟  
	UI* post;       //4개의 렌더타겟을 받아 그려줄

	class PlayerData* player;

	//Item* item;

	bool ChangeScene; 


public:
	Scene3();
	~Scene3();

	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};

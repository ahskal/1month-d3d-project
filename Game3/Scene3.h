#pragma once

class Scene3 : public Scene
{
private:
	//Camera* cam1;
	//Grid* grid;
	
	Deferred* deferred;   //4���� ����Ÿ��  
	UI* post;       //4���� ����Ÿ���� �޾� �׷���

	class PlayerData* player;

	//Item* item;

	bool ChangeScene; 


public:
	Scene3();
	~Scene3();

	virtual void Init() override;
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};

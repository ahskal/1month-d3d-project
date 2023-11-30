#pragma once

class PlayerData
{
public:
	static PlayerData* Create(string name = "PlayerData");
private:
	class Player* actor;
public:
	class UI_Player* UI;
	class PlayerObserver* pObserver;
	// �߰� ������ �ʵ��

	PlayerData();

	//���� ī�޶� ����
	void MainCamSet();

	void Update();
	void LateUpdate();

	void Hierarchy();

	//�⺻ UI Ȥ�� ���̴��� �����鼭 �ϴ� ������
	void Render(shared_ptr<Shader> pShader = nullptr);

	// ���۵� �������� ���� ��ü ������
	void DeferredRender(shared_ptr<Shader> pShader = nullptr);




};


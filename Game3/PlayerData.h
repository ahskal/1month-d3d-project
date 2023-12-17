#pragma once

class PlayerData : public Singleton<PlayerData>
{
public:
	class Player* actor;
	class UI_Player* UI;
	class PlayerObserver* pObserver;
	// �߰� ������ �ʵ��
	class Inventory* inventory;


	PlayerData();

	//���� ī�޶� ����
	void MainCamSet();

	void Update();
	void UIUpdate();
	void LateUpdate();

	void Hierarchy();

	//�⺻ UI Ȥ�� ���̴��� �����鼭 �ϴ� ������
	void Render(shared_ptr<Shader> pShader = nullptr);

	// ���۵� �������� ���� ��ü ������
	void DeferredRender(shared_ptr<Shader> pShader = nullptr);




};


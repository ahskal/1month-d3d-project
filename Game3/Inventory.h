#pragma once
class Inventory
{
private:
public:
	Inventory();
	~Inventory();
	std::vector<Item*> items;
	float Money;

	void AddItem(const Item* newItem);
	
	// ��Ÿ �κ��丮 ���� �޼ҵ�...

	void OpenList();

	void Update();

	void Hierarchy();

	void Render(shared_ptr<Shader> pShader = nullptr);
};


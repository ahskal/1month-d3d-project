#pragma once
class Inventory
{
private:
public:
	Inventory();
	~Inventory();
	std::vector<Item*> items;
	float Gold;

	void AddItem(Item* newItem);
	void AddItem(const Item* newItem);
	
	// 기타 인벤토리 관련 메소드...

	void OpenList();

	void Update();

	void Hierarchy();

	void Render(shared_ptr<Shader> pShader = nullptr);
};


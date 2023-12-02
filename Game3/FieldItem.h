#pragma once

class FieldItem : public Singleton<FieldItem>
{

public:
	void AddItem(const Item* newItem);
	void AddItem(const Item* newItem, Vector3 pos);
	// ��Ÿ �κ��丮 ���� �޼ҵ�...
	std::vector<Item*> items;

	vector<Item*>& ItemList() { return items; }
	void OpenList();

	void Update();

	void Hierarchy();

	void Render(shared_ptr<Shader> pShader = nullptr);

};


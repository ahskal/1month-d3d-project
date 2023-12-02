#pragma once

class FieldItem : public Singleton<FieldItem>
{

public:
	void AddItem(const Item* newItem);
	void AddItem(const Item* newItem, Vector3 pos);
	// 기타 인벤토리 관련 메소드...
	std::vector<Item*> items;

	vector<Item*>& ItemList() { return items; }
	void OpenList();

	void Update();

	void Hierarchy();

	void Render(shared_ptr<Shader> pShader = nullptr);

};


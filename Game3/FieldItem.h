#pragma once
class FieldItem : public Singleton<FieldItem>
{
	std::vector<Item*> items;

public:
	void CreateItem() {
	}

	
	void AddItem(const Item* newItem);
	// ��Ÿ �κ��丮 ���� �޼ҵ�...
	void OpenList();


};


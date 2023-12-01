#pragma once
class FieldItem : public Singleton<FieldItem>
{
	std::vector<Item*> items;

public:
	void CreateItem() {
	}

	
	void AddItem(const Item* newItem);
	// 기타 인벤토리 관련 메소드...
	void OpenList();


};


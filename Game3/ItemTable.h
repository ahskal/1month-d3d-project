#pragma once
class ItemTable : public Singleton<ItemTable>
{
private:
	std::unordered_map<std::string, Item> table;

public:
	// 아이템 테이블에 아이템 정보 추가
	void AddItemInfo(const std::string& name, float cost);

	// 아이템 생성 메서드
	Item* CreateItem(const std::string& name, ItemFactory* factory);

	void OpenList();

};


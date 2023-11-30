#pragma once
class ItemTable
{
private:
	std::unordered_map<ItemType, std::pair<std::string, float>> table;

public:
	// 아이템 테이블에 아이템 정보 추가
	void AddItemInfo(ItemType type, const std::string& name, float cost);
	
	// 아이템 생성 메서드
	Item* CreateItem(ItemType type, ItemFactory* factory);
	

};


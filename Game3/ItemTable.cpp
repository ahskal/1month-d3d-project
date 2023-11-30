#include "stdafx.h"
#include "ItemTable.h"

void ItemTable::AddItemInfo(ItemType type, const std::string& name, float cost)
{
	table[type] = std::make_pair(name, cost);
}

Item* ItemTable::CreateItem(ItemType type, ItemFactory* factory)
{
	auto it = table.find(type);
	if (it != table.end()) {
		Item* newItem = factory->CreateItem();
		newItem->ItemName = it->second.first;
		newItem->Cost = it->second.second;
		return newItem;
	}
	return nullptr; // 유효하지 않은 아이템 타입인 경우
}
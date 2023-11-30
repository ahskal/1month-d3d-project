#include "stdafx.h"
#include "ItemTable.h"

void ItemTable::AddItemInfo(const std::string& name, float cost)
{
    table[name] = Item{ name, cost };
}

Item* ItemTable::CreateItem(const std::string& name, ItemFactory* factory)
{
    auto it = table.find(name);
    if (it != table.end()) {
        Item* newItem = factory->CreateItem();
        newItem->ItemName = it->second.ItemName;
        newItem->Cost = it->second.Cost;
        return newItem;
    }
    return nullptr; // 유효하지 않은 아이템 이름인 경우
}

#include "stdafx.h"
#include "ItemTable.h"

void ItemTable::AddItemInfo(const std::string& name, float cost)
{
	Item newItem(name, cost); // 직접 Item 객체를 생성하고 값을 할당

   // 이미 아이템이 존재하는지 확인
	auto it = table.find(name);
	if (it != table.end()) {
		// 중복된 아이템이 있으면 추가 안함
		cout << "아이템 중복 " << endl;
		cout << "해당 아이템 : " << name << endl;
	}
	else {
		// 새로운 아이템 추가
		table[name] = newItem;
	}
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

void ItemTable::OpenList()
{
	for (auto um : table) {
		cout << "[아이템 이름]\t[아이템 비용]" << endl;
		int size = um.second.ItemName.size();
		if(um.second.ItemName.size() > 8)
			
		cout << um.second.ItemName << "\t" << um.second.Cost << endl;
		else {
			cout << um.second.ItemName << "\t\t" << um.second.Cost << endl;

		}
	}
}

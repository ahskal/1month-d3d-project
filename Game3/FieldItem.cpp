#include "stdafx.h"
#include "FieldItem.h"

void FieldItem::AddItem(const Item* newItem)
{
	auto it = std::find_if(items.begin(), items.end(), [&newItem](const Item* existingItem) {
		return existingItem->ItemName == newItem->ItemName;
		});

	if (it != items.end()) {
		(*it)->size++; // ���� �������� ������ ���� ����
	}
	else {
		Item* NewItme = new Item(*newItem);
		NewItme->size++;
		items.push_back(NewItme);
	}
}

void FieldItem::OpenList()
{
	for (auto inven : items) {
		cout << "[������ �̸�]\t[������ ���]\t[������ ����]" << endl;
		int size = inven->ItemName.size();
		if (inven->ItemName.size() > 8)
			cout << inven->ItemName << "\t" << inven->Cost << "\t\t" << inven->size << endl;
		else {
			cout << inven->ItemName << "\t\t" << inven->Cost << "\t\t" << inven->size << endl;
		}
	}
}
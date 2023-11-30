#include "stdafx.h"
#include "ItemTable.h"

void ItemTable::AddItemInfo(const std::string& name, float cost)
{
	Item newItem(name, cost); // ���� Item ��ü�� �����ϰ� ���� �Ҵ�

   // �̹� �������� �����ϴ��� Ȯ��
	auto it = table.find(name);
	if (it != table.end()) {
		// �ߺ��� �������� ������ �߰� ����
		cout << "������ �ߺ� " << endl;
		cout << "�ش� ������ : " << name << endl;
	}
	else {
		// ���ο� ������ �߰�
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
	return nullptr; // ��ȿ���� ���� ������ �̸��� ���
}

void ItemTable::OpenList()
{
	for (auto um : table) {
		cout << "[������ �̸�]\t[������ ���]" << endl;
		int size = um.second.ItemName.size();
		if(um.second.ItemName.size() > 8)
			
		cout << um.second.ItemName << "\t" << um.second.Cost << endl;
		else {
			cout << um.second.ItemName << "\t\t" << um.second.Cost << endl;

		}
	}
}

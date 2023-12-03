#include "stdafx.h"
#include "Inventory.h"

Inventory::Inventory()
{
	items.resize(0);
	Gold = 0;
}

Inventory::~Inventory()
{
}

void Inventory::AddItem(Item* newItem)
{
	auto it = std::find_if(items.begin(), items.end(), [newItem](const Item* existingItem) {
		return existingItem->ItemName == newItem->ItemName;
		});


	if (it != items.end()) {
		(*it)->size += newItem->size; // ���� �������� ������ ���� ����
	}
	else {
		// newItem�� moneyItem�� �Ļ� Ŭ�������� Ȯ��
		Item* NewItem = new Item(*newItem);
		MoneyItem* moneyItemPtr = dynamic_cast<MoneyItem*>(newItem);
		if (moneyItemPtr) {
			Gold += moneyItemPtr->Cost;
			cout << " ++ " << endl;
		}
		else {
			// newItem�� moneyItem�� �Ļ� Ŭ������ �ƴ� ����� ó��
			NewItem->state = ItemState::Inven;
			NewItem->actor->name = newItem->ItemName;
			items.push_back(NewItem);
		}
	}
}

void Inventory::OpenList()
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

void Inventory::Update()
{
	ImGui::Text("Gold : %.2f", Gold);
	for (auto inven : items) {
		inven->Update();
		//inven->actor->RenderHierarchy();
	}
}

void Inventory::Hierarchy()
{
	for (auto inven : items) {
		inven->actor->RenderHierarchy();
	}
}

void Inventory::Render(shared_ptr<Shader> pShader)
{
	for (auto inven : items) {
		inven->Render(pShader);
	}
}

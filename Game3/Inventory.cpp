#include "stdafx.h"
#include "Inventory.h"

Inventory::Inventory()
{
	items.resize(0);
}

Inventory::~Inventory()
{
}

void Inventory::AddItem(const Item* newItem)
{
	auto it = std::find_if(items.begin(), items.end(), [newItem](const Item* existingItem) {
		return existingItem->ItemName == newItem->ItemName;
		});
	
	

	if (it != items.end()) {
		(*it)->size += newItem->size; // ���� �������� ������ ���� ����
	}
	else {
		Item* NewItem = new Item(*newItem);
		NewItem->state = ItemState::Inven;
		NewItem->actor->name = newItem->ItemName;
		NewItem->size++;
		items.push_back(NewItem);
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
	for (auto inven : items) {
		inven->Update();
		inven->actor->RenderHierarchy();
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

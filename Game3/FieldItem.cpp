#include "stdafx.h"
#include "FieldItem.h"

void FieldItem::AddItem(const Item* newItem)
{
	auto it = std::find_if(items.begin(), items.end(), [&newItem](const Item* existingItem) {
		return existingItem->ItemName == newItem->ItemName;
		});

	if (it != items.end()) {
		(*it)->size++; // 기존 아이템이 있으면 수량 증가
	}
	else {
		Item* NewItem = new Item(*newItem);
		NewItem->size++;
		items.emplace_back(NewItem);
	}
}

void FieldItem::AddItem(Item* newItem, Vector3 pos)
{
	newItem->state = ItemState::Drop;
	newItem->actor->SetWorldPos(pos);
	newItem->size++;
	items.emplace_back(newItem);
}

void FieldItem::OpenList()
{
	for (auto inven : items) {
		cout << "[아이템 이름]\t[아이템 비용]\t[아이템 갯수]" << endl;
		int size = inven->ItemName.size();
		if (inven->ItemName.size() > 8)
			cout << inven->ItemName << "\t" << inven->Cost << "\t\t" << inven->size << endl;
		else {
			cout << inven->ItemName << "\t\t" << inven->Cost << "\t\t" << inven->size << endl;
		}
	}
}

void FieldItem::Update()
{
	for (auto inven : items) {
		inven->Update();
	}
}

void FieldItem::Hierarchy()
{
	for (auto inven : items) {
		inven->actor->RenderHierarchy();
	}
}

void FieldItem::Render(shared_ptr<Shader> pShader)
{
	for (auto inven : items) {
		inven->Render(pShader);
	}
}

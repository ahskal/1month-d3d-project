#include "stdafx.h"
#include "Item.h"

Item::Item()
	: Cost(0.0f), size(0)
{
	actor = nullptr;
	state = ItemState::Inven;
}

Item::Item(const std::string& itemName, float itemCost)
	: ItemName(itemName), Cost(itemCost), size(0)
{
	actor = nullptr;
	state = ItemState::Inven;
}

Item::~Item()
{
	SafeRelease(actor);
}

void Item::ShowItemInfo() const
{
	std::cout << "Item Name: " << ItemName << ", Cost: " << Cost << std::endl;
}

void Item::Update()
{
	if (state == ItemState::Drop)
		actor->Update();
}

void Item::Render(shared_ptr<Shader> pShader)
{
	if (state == ItemState::Drop)
		actor->Render(pShader);
}


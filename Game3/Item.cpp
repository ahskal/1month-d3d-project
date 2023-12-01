#include "stdafx.h"
#include "Item.h"

Item::Item()
	: Cost(0.0f)
{
	actor = Actor::Create();
	actor->LoadFile("Unit/Item.xml");

	state = ItemState::Drop;
}

Item::Item(const std::string& itemName, float itemCost)
	: ItemName(itemName), Cost(itemCost)
{

}

void Item::ShowItemInfo() const
{
	std::cout << "Item Name: " << ItemName << ", Cost: " << Cost << std::endl;
}

void Item::Update()
{
	if (state == ItemState::Drop)
		actor->Update();
	else {

	}
}

void Item::Render()
{
	if (state == ItemState::Drop)
		actor->Render();
	else {

	}
}


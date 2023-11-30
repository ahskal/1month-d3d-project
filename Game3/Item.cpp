#include "stdafx.h"
#include "Item.h"

Item::Item()
	: Cost(0.0f)
{

}

Item::Item(const std::string& itemName, float itemCost)
	: ItemName(itemName), Cost(itemCost)
{
	
}

void Item::ShowItemInfo() const
{
	std::cout << "Item Name: " << ItemName << ", Cost: " << Cost << std::endl;
}


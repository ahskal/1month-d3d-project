#include "stdafx.h"
#include "Item.h"

Item::Item()
{
}

void Item::ShowItemInfo() const
{
	std::cout << "Item Name: " << ItemName << ", Cost: " << Cost << std::endl;
}


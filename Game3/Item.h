#pragma once

enum class ItemType {
	None,
	Root,
	EndSize
};

class Item
{
private:
	friend class ItemTable;
protected:
	
public:
	std::string ItemName;
	float Cost;

	Item();
	Item(const std::string& itemName, float itemCost);
	
	virtual ~Item() = default;

	void ShowItemInfo() const;
};


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
	std::string ItemName;
	float Cost;

public:
	Item();
	virtual ~Item() = default;

	void ShowItemInfo() const;
};


#pragma once

enum class ItemType {
	None,
	Root,
	EndSize
};

enum class ItemState {
	Drop,
	Inven
};

class Item
{
private:
	friend class ItemTable;
protected:
	
public:
	class Actor* actor;
	ItemState state;

	std::string ItemName;
	float Cost;
	float size;

	Item();
	Item(const std::string& itemName, float itemCost);
	
	virtual ~Item() = default;
	void ShowItemInfo() const;

	void Update();
	void Render();

};


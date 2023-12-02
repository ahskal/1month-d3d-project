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

	Actor* actor;
	ItemState state;

	std::string ItemName;
	float Cost;
	float size;

	Item();
	Item(const std::string& itemName, float itemCost);
	~Item();
	
	void ShowItemInfo() const;

	void Update();
	void Render(shared_ptr<Shader> pShader);
};

class RootItem : public Item
{
public:
	RootItem() {
		actor = Actor::Create();
		actor->LoadFile("Item/Item.xml");
	};
};

class MoneyItem : public Item
{
public:
	MoneyItem() {
		actor = Actor::Create();
		actor->LoadFile("Item/GoldSpot.xml");
	};
};


#pragma once
class ItemTable
{
private:
	std::unordered_map<ItemType, std::pair<std::string, float>> table;

public:
	// ������ ���̺� ������ ���� �߰�
	void AddItemInfo(ItemType type, const std::string& name, float cost);
	
	// ������ ���� �޼���
	Item* CreateItem(ItemType type, ItemFactory* factory);
	

};


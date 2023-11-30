#pragma once
class ItemTable : public Singleton<ItemTable>
{
private:
	std::unordered_map<std::string, Item> table;

public:
	// ������ ���̺� ������ ���� �߰�
	void AddItemInfo(const std::string& name, float cost);

	// ������ ���� �޼���
	Item* CreateItem(const std::string& name, ItemFactory* factory);

	void OpenList();

};


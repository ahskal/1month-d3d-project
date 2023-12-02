#pragma once
class ItemFactory
{
public:
    virtual ~ItemFactory() = default;

    // ���丮 �޼ҵ�
    virtual Item* CreateItem() = 0;
};

class RootItemFactory : public ItemFactory
{
    Item* CreateItem() override {
        return new RootItem(); // �⺻ ���� ���, �ʿ信 ���� ���� ����
    }
};

class MoneyItemFactory : public ItemFactory
{
    Item* CreateItem() override {
        return new MoneyItem(); // �⺻ ���� ���, �ʿ信 ���� ���� ����
    }
};
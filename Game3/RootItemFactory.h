#pragma once
class RootItemFactory : public ItemFactory
{
    Item* CreateItem() override {
        return new RootItem(); // �⺻ ���� ���, �ʿ信 ���� ���� ����
    }
};


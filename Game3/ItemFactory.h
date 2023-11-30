#pragma once
class ItemFactory
{
public:
    virtual ~ItemFactory() = default;

    // ���丮 �޼ҵ�
    virtual Item* CreateItem() = 0;
};

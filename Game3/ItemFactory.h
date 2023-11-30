#pragma once
class ItemFactory
{
public:
    virtual ~ItemFactory() = default;

    // 팩토리 메소드
    virtual Item* CreateItem() = 0;
};

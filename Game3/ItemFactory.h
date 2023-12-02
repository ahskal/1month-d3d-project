#pragma once
class ItemFactory
{
public:
    virtual ~ItemFactory() = default;

    // 팩토리 메소드
    virtual Item* CreateItem() = 0;
};

class RootItemFactory : public ItemFactory
{
    Item* CreateItem() override {
        return new RootItem(); // 기본 생성 방식, 필요에 따라 변경 가능
    }
};

class MoneyItemFactory : public ItemFactory
{
    Item* CreateItem() override {
        return new MoneyItem(); // 기본 생성 방식, 필요에 따라 변경 가능
    }
};
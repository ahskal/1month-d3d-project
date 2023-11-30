#pragma once
class RootItemFactory : public ItemFactory
{
    Item* CreateItem() override {
        return new RootItem(); // 기본 생성 방식, 필요에 따라 변경 가능
    }
};


#pragma once
class Inventory
{
private:
	std::vector<Item*> items;

public:
	void AddItem(const Item* newItem) {
		auto it = std::find_if(items.begin(), items.end(), [&newItem](const Item* existingItem) {
			return existingItem->ItemName == newItem->ItemName;
			});

		if (it != items.end()) {
			(*it)->size++; // 기존 아이템이 있으면 수량 증가
		}
		else {
			Item* NewItme = new Item(*newItem);
			NewItme->size++;
			items.push_back(NewItme);
		}
	}
	// 기타 인벤토리 관련 메소드...

	void OpenList() {
		for (auto inven : items) {
			cout << "[아이템 이름]\t[아이템 비용]\t[아이템 갯수]" << endl;
			int size = inven->ItemName.size();
			if (inven->ItemName.size() > 8)
				cout << inven->ItemName << "\t" << inven->Cost << "\t\t" << inven->size << endl;
			else {
				cout << inven->ItemName << "\t\t" << inven->Cost << "\t\t" << inven->size << endl;
			}
		}
	}

};


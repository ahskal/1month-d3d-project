#pragma once

class PlayerObserver : public Observer
{
private:
	class Player* player;
	string observerName;

public:
	PlayerObserver(Player* data = nullptr, string name = "Observer");
	void Update(const string& message);

	// void*�� �ٷ�� ���� getter�� setter �߰�
	Player* GetData() const { return player; }
	void SetData(Player* newData) { player = newData; }
};

#pragma once

class PlayerObserver : public Observer
{
private:
	class Player* player;
	string observerName;

public:
	PlayerObserver(Player* data = nullptr, string name = "Observer");
	void Update(const string& message);

	// void*를 다루기 위한 getter와 setter 추가
	Player* GetData() const { return player; }
	void SetData(Player* newData) { player = newData; }
};

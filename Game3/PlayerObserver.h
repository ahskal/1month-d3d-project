#pragma once

class PlayerObserver : public Observer
{
private:
	class Player* player;
	string observerName;

public:
	PlayerObserver(Player* data = nullptr, string name = "P_Observer");
	void Update(const string& message);
	void Update(const Vector3& position);
	
	// void*�� �ٷ�� ���� getter�� setter �߰�
	Player* GetData() const { return player; }
	void SetData(Player* newData) { player = newData; }

	Vector3 GetPos() { return player->GetWorldPos(); }
	Collider* GetCol() { return player->collider; }
	GameObject* GetSword() { return player->Find("sword"); }
};

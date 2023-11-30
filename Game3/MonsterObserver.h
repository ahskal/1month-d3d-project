#pragma once

class MonsterObserver : public Observer
{
private:
	class Monster* monster;
	string observerName;

public:
	MonsterObserver(Monster* data = nullptr, string name = "M_Observer");
	void Update(const string& message);	

	// void*�� �ٷ�� ���� getter�� setter �߰�
	Monster* GetData() const { return monster; }
	void SetData(Monster* newData) { monster = newData; }

	Vector3 GetPos() const { return monster->GetWorldPos(); }
};
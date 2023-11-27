#pragma once
#include "stdafx.h";
#include "Monster.h"
#include "Observer.h"
class MonsterObserver : public Observer
{
private:
	class Monster* monster;
	Status status;
	string observerName;
public:
	MonsterObserver(Monster* monster) : monster(monster), observerName("") {
		status.Attack = 0;
		status.Defense = 0;
		status.MaxHp = 0;
		status.Hp = 0;
		status.MaxSp = 0;
		status.Sp = 0;
	}

	void Update(const string& message) {
		cout << "Observer " << observerName << " received message : " << message << endl;
	}

	void Update(const Status& status) {
		this->status = status;
		//system("cls");
		//// ���� ����� Player Ŭ�������� ������ �����ͼ� ���
		//cout << "< PLAYER >" << endl;
		//cout << "HP : " << status.Hp << " / " << status.MaxHp << endl;
		//cout << "SP : " << status.Hp << " / " << status.MaxHp << endl;
		//cout << "Attack : " << status.Attack << endl;
		//cout << "Defense : " << status.Defense << endl;
		//// ���¿� ���� �۾� ����
	}

	Status GetStatus() { return status; }


};


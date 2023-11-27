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
		//// 관찰 대상인 Player 클래스에서 스텟을 가져와서 사용
		//cout << "< PLAYER >" << endl;
		//cout << "HP : " << status.Hp << " / " << status.MaxHp << endl;
		//cout << "SP : " << status.Hp << " / " << status.MaxHp << endl;
		//cout << "Attack : " << status.Attack << endl;
		//cout << "Defense : " << status.Defense << endl;
		//// 상태에 대한 작업 수행
	}

	Status GetStatus() { return status; }


};


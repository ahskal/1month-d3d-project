#pragma once
#include "Unit.h"
#include "Observer.h"

class Subject : public Unit
{
public:
	static Subject* Create(string name = "Subject") {
		Subject* temp = new Subject();
		temp->name = name;
		temp->type = ObType::Actor;
		return temp;
	};
private:
	vector<Observer*> observers;
	string StateName;
public:
	Subject() {

	};
	~Subject() {
	
	};

	void Attach(Observer* observer) {
		observers.push_back(observer);
	}
	void Detach(Observer* observer) {
		observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
	}
	void SetState(const string& newState) {
		StateName = newState;
		NotifyObservers();
	}
	void NotifyObservers() {
		for (Observer* observer : observers) {
			observer->Update(StateName);
		}
	}

};




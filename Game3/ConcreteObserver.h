#pragma once
#include "stdafx.h";
#include "Observer.h"

class ConcreteObserver : public Observer
{
private:
	string observerName;

public :
	ConcreteObserver(const string& name): observerName(name){}

	void Update(const string& message) override {
		cout << "Observer " << observerName << " received message : " << endl;
	}
};


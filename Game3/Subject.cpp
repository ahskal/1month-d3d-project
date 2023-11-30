#include "stdafx.h"
#include "Unit.h"
#include "Observer.h"
#include "Subject.h"

Subject* Subject::Create(string name)
{
	Subject* temp = new Subject();
	temp->name = name;
	temp->type = ObType::Actor;
	return temp;
};
Subject::Subject()
{
}

Subject::~Subject()
{
}

void Subject::Attach(Observer* observer)
{
	observers.push_back(observer);
}

void Subject::Detach(Observer* observer)
{
	observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
}

void Subject::SetState(const string& message)
{
	this->message = message;
	NotifyMessageObservers();
}

void Subject::NotifyMessageObservers()
{
	for (Observer* observer : observers) {
		observer->Update(message);
	}
}

void Subject::SetState(const Vector3& position)
{
	this->position = position;
	NotifyPosObservers();
}

void Subject::NotifyPosObservers()
{
	for (Observer* observer : observers) {
		observer->Update(position);
	}
}

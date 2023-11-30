#pragma once

class Subject : public Unit
{
public:
	static Subject* Create(string name = "Subject");
private:
	vector<class Observer*> observers;
	string message;
public:
	Subject();
	~Subject();

	void Attach(class Observer* observer);
	void Detach(class Observer* observer);
	void SetState(const string& message);
	void NotifyObservers();
};
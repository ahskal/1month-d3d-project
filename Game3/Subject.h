#pragma once

class Subject : public Unit
{
public:
	static Subject* Create(string name = "Subject");
private:
	vector<class Observer*> observers;
	string message;
	Vector3 position;
public:
	Subject();
	~Subject();

	void Attach(class Observer* observer);
	void Detach(class Observer* observer);
	void SetState(const string& message);
	void NotifyMessageObservers();

	void SetState(const Vector3& position);
	void NotifyPosObservers();
};
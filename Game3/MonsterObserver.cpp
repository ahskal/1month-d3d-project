#include "Observer.h"
#include "stdafx.h"
#include "Monster.h"
#include "MonsterObserver.h"

MonsterObserver::MonsterObserver(Monster* data, string name)
	: observerName(name), monster(data)
{
}

void MonsterObserver::Update(const string& message)
{
	float WTime = TIMER->GetWorldTime();
	cout << "Observer " << observerName << endl;
	cout << "[" << std::fixed << std::setprecision(2) << WTime << "]" << " received message : " << message << endl;
}
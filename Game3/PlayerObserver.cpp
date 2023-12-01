#include "stdafx.h"
#include "Observer.h"
#include "Player.h"
#include "PlayerObserver.h"

extern bool DEBUG_MODE;
extern bool NONE_SCENE;
extern bool TEXT_LOG  ;
extern bool FREE_CAM  ;


PlayerObserver::PlayerObserver(Player* data, string name)
	: observerName(name), player(data)
{
}

void PlayerObserver::Update(const string& message)
{
	if (TEXT_LOG) {
		float WTime = TIMER->GetWorldTime();
		cout << "Observer " << observerName << endl;
		cout << "[" << std::fixed << std::setprecision(2) << WTime << "]" << " messageCall : "
			<< message << endl;
	}
}

void PlayerObserver::Update(const Vector3& position)
{
	if (TEXT_LOG) {
		float WTime = TIMER->GetWorldTime();
		cout << "Observer " << observerName << endl;
		cout << "[" << std::fixed << std::setprecision(2) << WTime << "]" << " messageCall : "
			<< position.x << "," << position.y << "," << position.z << endl;
	}
}

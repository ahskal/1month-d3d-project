#include "stdafx.h"
#include "Player.h"
#include "UI_Player.h"
#include "PlayerObserver.h"
#include "Inventory.h"
#include "PlayerData.h"

PlayerData::PlayerData()
{
	actor = Player::Create();
	actor->LoadFile("Unit/Man.xml");

	pObserver = new PlayerObserver(actor);
	actor->Attach(pObserver);

	UI = UI_Player::Create();
	UI->LoadFile("UI/Main_Layout_1.xml");
	UI->SetData(actor);

	inventory = new Inventory();
}

void PlayerData::MainCamSet()
{
	Camera::main = static_cast<Camera*>(actor->Find("Camera"));
}

void PlayerData::Update()
{
	actor->Update();
	UI->Update();
	inventory->Update();
}

void PlayerData::LateUpdate()
{
	actor->LateUpdate();
}

void PlayerData::Hierarchy()
{
	actor->RenderHierarchy();
	UI->RenderHierarchy();
}

void PlayerData::Render(shared_ptr<Shader> pShader)
{
	int l = App.GetWidth() - 120.0f;
	int t = App.GetHeight() - 40.0f;
	int r = App.GetWidth();
	int b = App.GetHeight();

	wstringstream ss;
	ss << L" HP : " << fixed << setprecision(2) << actor->Hp;
	wstring N = ss.str();
	DWRITE->RenderText(N, RECT{ 0,t,1000,b }, 20, L"πŸ≈¡√º", Color(1, 1, 1, 1));
	t = App.GetHeight() - 20.0f;
	wstringstream sss;
	sss << L" SP : " << fixed << setprecision(2) << actor->Sp;
	N = sss.str();
	DWRITE->RenderText(N, RECT{ 0,t,1000,b }, 20, L"πŸ≈¡√º", Color(1, 1, 1, 1));

	wstringstream ssss;
	ssss << L" FPS : " << fixed << setprecision(2) << TIMER->GetFramePerSecond();
	N = ssss.str();
	DWRITE->RenderText(N, RECT{ l,0,r,300 }, 20, L"πŸ≈¡√º", Color(1, 1, 1, 1));

	
	UI->Render(pShader);
	inventory->Render(pShader);
}

void PlayerData::DeferredRender(shared_ptr<Shader> pShader)
{
	actor->Render(pShader);
}

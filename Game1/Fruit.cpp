#include "stdafx.h"
#include "Fruit.h"

Fruit* Fruit::Create(string name)
{
	Fruit* temp = new Fruit();
	temp->LoadFile("bullet.xml");
	temp->type = ObType::Actor;
	return temp;
}

Fruit::Fruit()
{
}

Fruit::~Fruit()
{
}

void Fruit::Update()
{
	if (not isFire) return;

	SetWorldPos(Utility::Lerp(p0, p1,p2, time));

	time += power * DELTA;

	if (time >= 1)
	{
		isFire = false;
	}


	if (not Find("Body")->visible)
	{
		Find("L_Body")->MoveWorldPos(
			Find("L_Body")->GetUp() * boomPower * DELTA);
		Find("R_Body")->MoveWorldPos(
			Find("R_Body")->GetUp() * boomPower * DELTA);
		boomPower -= DELTA;
		if (boomPower < 0.0f) boomPower = 0.0f;
	}


	Actor::Update();

	if (GetWorldPos().y < -5)
	{
		isFire = false;
	}

}

void Fruit::Render(shared_ptr<Shader> pShader)
{
	if (not isFire)  
		return ;

	Actor::Render(pShader);
}

bool Fruit::Fire(Vector3 pos, Vector3 dir, float power)
{
	if (isFire)  return false;

	SetWorldPos(pos);
	isFire = true;
	this->dir = dir;
	this->power = power;

	gravity = 0.0f;
	Find("Body")->visible = true;
	Find("L_Body")->visible = false;
	Find("R_Body")->visible = false;
	Find("L_Body")->SetLocalPos(Vector3());
	Find("R_Body")->SetLocalPos(Vector3());
	return true;
}

bool Fruit::Fire2(Vector3 p0, Vector3 p1, Vector3 p2, float power)
{
	if (isFire)  return false;

	SetWorldPos(p0);
	isFire = true;

	Vector3 temp = p2 - p0;
	float dis = temp.Length();

	time = 0.0f;
	this->power =  power / dis;
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;



	return true;
}

void Fruit::Boom()
{
	//cout << "boom!" << endl;
	if (not Find("Body")->visible) return;

	Find("Body")->visible = false;
	Find("L_Body")->visible = true;
	Find("R_Body")->visible = true;
	boomPower = 2.0f;
	power = 0.0f;
	gravity *= 0.5f;
	
	rotation.z = 90.0f * ToRadian -atan2f(INPUT->movePosition.y, INPUT->movePosition.x);
}

#include "stdafx.h"
#include "Zamong.h"
#include "Fruit.h"

Zamong* Zamong::Create(string name)
{
	Zamong* temp = new Zamong();
	temp->LoadFile("Vam.xml");
	temp->type = ObType::Actor;
	Camera::main = (Camera*)temp->Find("Cam");

	Camera::main->viewport.x = 0.0f;
	Camera::main->viewport.y = 0.0f;
	Camera::main->viewport.width = App.GetWidth();
	Camera::main->viewport.height = App.GetHeight();
	Camera::main->width = App.GetWidth();
	Camera::main->height = App.GetHeight();

	temp->state = ZamongState::IDLE;
	temp->rotTime = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		temp->fruit[i] = Fruit::Create();
	}


	temp->Find("mixamorig:Hips")->rootMotion = true;

	return temp;
}

Zamong* Zamong::Create(Zamong* src)
{
	Zamong* temp = new Zamong(*src);
	temp->type = ObType::Actor;
	temp->state = ZamongState::IDLE;
	temp->rotTime = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		temp->fruit[i] = Fruit::Create();
	}
	temp->CopyChild(src);

	return temp;
}

Zamong::Zamong()
{
	
}

Zamong::~Zamong()
{
}

void Zamong::Update()
{
	last = GetWorldPos();
	


	if (state == ZamongState::IDLE)
	{

		if (INPUT->KeyPress('W') or
			INPUT->KeyPress('A') or
			INPUT->KeyPress('S') or
			INPUT->KeyPress('D'))
		{
			state = ZamongState::MOVE;
			anim->ChangeAnimation(AnimationState::LOOP, 1, 0.1f);
		}

	}
	if (state == ZamongState::MOVE)
	{
		/*if (INPUT->KeyPress('W'))
		{
			MoveWorldPos(GetForward() * 3.0f * DELTA);
		}
		if (INPUT->KeyPress('S'))
		{
			MoveWorldPos(-GetForward() * 3.0f * DELTA);
		}

		if (INPUT->KeyPress('A'))
		{
			rotation.y -= DELTA;
		}
		if (INPUT->KeyPress('D'))
		{
			rotation.y += DELTA;
		}


		if (not(INPUT->KeyPress('W') or
			INPUT->KeyPress('A') or
			INPUT->KeyPress('S') or
			INPUT->KeyPress('D')))
		{
			state = ZamongState::IDLE;
			anim->ChangeAnimation(AnimationState::LOOP, 0, 0.1f);
		}*/



		//MoveWorldPos(dir * speed * DELTA);

		if (anim->GetPlayTime() >= 1.0f)
		{
			/*Vector3 minus = Find("mixamorig:Hips")->GetWorldPos() - last;
			minus.y = 0.0f;
			MoveWorldPos(minus);
			Transform::Update();
			last = Find("mixamorig:Hips")->GetWorldPos();
			anim->ChangeAnimation(AnimationState::ONCE_LAST, 1, 0.0f);*/
		}

		/*time -= DELTA;

		if (time < 0.0f)
		{
			state = ZamongState::IDLE;
		}*/

	}
	if (rotTime > 0.0f)
	{
		rotation.y += rotSpeed * DELTA;
		rotTime -= DELTA;
	}
	
	/*Quaternion q1,q2;
	q1 = Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.y, rotation.y);
	q2 = Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.y, rotation.y);
	q1 = Quaternion::Lerp(q1, q2,0.3f);*/


	for (int i = 0; i < 3; i++)
	{
		fruit[i]->Update();
	}
	Actor::Update();
}

void Zamong::Render(shared_ptr<Shader> pShader)
{
	Actor::Render(pShader);
	for (int i = 0; i < 3; i++)
	{
		fruit[i]->Render(pShader);
	}
}

void Zamong::Move(Vector3 Target)
{
	last = Find("mixamorig:Hips")->GetWorldPos();
	state = ZamongState::MOVE;
	anim->ChangeAnimation(AnimationState::LOOP, 1, 0.0f);
	dir = Target - GetWorldPos();
	
	speed = 30.0f;
	//예상 도착 시간
	time = dir.Length() / speed;


	dir.Normalize();
	float srcRot = atan2f(GetForward().x, GetForward().z);
	//cout << "현재 각도:"<< srcRot / ToRadian << endl;
	float destRot = atan2f(dir.x, dir.z) ;
	//cout << "바라볼 각도:"<< destRot / ToRadian << endl;

	float dis = destRot - srcRot;

	if (dis > PI)
	{
		srcRot += PI * 2.0f;
	}
	else if (dis < -PI)
	{
		srcRot -= PI * 2.0f;
	}
	dis = destRot - srcRot;

	//회전력
	rotSpeed = dis > 0 ?  PI : -PI;

	//출발각도를 맞춰준다.
	rotation.y = srcRot;

	//회전시간
	rotTime = fabs(dis) / fabs(rotSpeed);

	

}

void Zamong::Fire( Vector3 dest, float power)
{
	Vector3 Forward = dest - GetWorldPos();
	float halfDis = Forward.Length() * 0.5f;
	Forward.Normalize();
	Vector3 Up = Vector3(0, 1, 0);
	Vector3 Right = Forward.Cross(Up);

	Vector3 R = GetWorldPos() + Forward * halfDis*0.5f + Right * halfDis;
	Vector3 L = GetWorldPos() + Forward * halfDis*0.5f - Right * halfDis;


	fruit[0]->Fire2(GetWorldPos(), L, dest, power);
	fruit[1]->Fire2(GetWorldPos(), R, dest, power);
}

void Zamong::WolrdUpdate()
{
	GameObject::Update();
}


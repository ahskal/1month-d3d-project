#pragma once
#include "../2DFrameWork/framework.h"
#pragma comment (lib,"../Build/2DFrameWork/2DFrameWork.lib")

#include "Unit.h"

#include "MonsterManager.h"

#define MonMGR	MonsterManager::GetInstance()

extern bool DEBUG_MODE;

struct Status {
	float Hp, MaxHp;
	float Sp, MaxSp;
	float Attack;
	float Defense;
};



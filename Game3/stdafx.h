#pragma once
#include "../2DFrameWork/framework.h"
#pragma comment (lib,"../Build/2DFrameWork/2DFrameWork.lib")

#include "Unit.h"
#include "Subject.h"
#include "Observer.h"

#include "Item.h"

#include "ItemFactory.h"

#include "MonsterManager.h"
#include "ItemTable.h"
#include "FieldItem.h"

#include "DamageText.h"
#include "DamageFontManager.h"

#include "PlayerData.h"

#define ITEM	ItemTable::GetInstance()
#define FIELD   FieldItem::GetInstance()
#define DAMAGEFONT	DamageFontManager::GetInstance()

#define MONSTER	MonsterManager::GetInstance()
#define PLAYER PlayerData::GetInstance()
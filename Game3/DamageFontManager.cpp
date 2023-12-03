#include "stdafx.h"
#include "DamageFontManager.h"

void DamageFontManager::AddDamageText(int value, Vector3 spawnPos)
{
	string valueStr = to_string(value);
	Billboard* parent = nullptr;
	for (int i = 0; i < valueStr.size(); i++)
	{
		value = valueStr[i] - '0';
		DamageText* damageText = new DamageText(value, spawnPos);
		damageText->SetParentNode(parent);
		damageTexts.emplace_back(damageText);

		parent = damageText->GetBillboard();
	}
}

void DamageFontManager::Update()
{
	for (auto iter = damageTexts.begin(); iter != damageTexts.end();)
	{
		if ((*iter)->IsTimeOver())
		{
			(*iter)->Release();
			iter = damageTexts.erase(iter);
		}
		else iter++;
	}

	for (auto it : damageTexts)
	{
		it->Update();
	}
};

void DamageFontManager::Render(shared_ptr<Shader> pShader)
{
	for (auto it : damageTexts) {
		it->Render(pShader);
	}
}
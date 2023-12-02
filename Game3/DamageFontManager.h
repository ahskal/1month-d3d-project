#pragma once
class DamageFontManager : public Singleton<DamageFontManager>
{
public:
	deque<DamageText*> damageTexts;

	void Update();
	void Render(shared_ptr<Shader> pShader = nullptr);	
	void AddDamageText(int value, Vector3 spawnPos);
};
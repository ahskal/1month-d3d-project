#pragma once
#include "stdafx.h"

class UI_Monster : public Billboard
{
public:
	static UI_Monster* Create(string name = "UI_Monster");
private:
	Status status;

public:
	UI_Monster();
	~UI_Monster();
	void SetStatus(Status status);
	void Update() override;
	void Render(shared_ptr<Shader> pShader = nullptr) override;

};


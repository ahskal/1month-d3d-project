#pragma once
#include "stdafx.h"

class Observer
{
public:
	virtual void Update(const string& maeesage) = 0;
	virtual void Update(const Status& status) = 0;
};


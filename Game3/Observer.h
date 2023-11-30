#pragma once

#include <string>

class Observer
{
public:
	Observer();
	virtual ~Observer();
	virtual void Update(const std::string& message) = 0;
	virtual void Update(const Vector3& position) = 0;
};


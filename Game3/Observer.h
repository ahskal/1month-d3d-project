#pragma once

#include <string>

class Observer
{
protected:
public:
    Observer();
    virtual ~Observer();
    virtual void Update(const std::string& message) = 0;
};


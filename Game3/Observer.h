#pragma once

class Observer
{
protected:
public:
    Observer();
    virtual ~Observer();
    virtual void Update(const string& message) = 0;
};


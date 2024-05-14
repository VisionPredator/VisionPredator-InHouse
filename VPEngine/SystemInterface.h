#pragma once
class IUpdatable
{
public:
	virtual void Update(float deltaTime) = 0;
};

class IFixedUpdatable
{
public:
	virtual void FixedUpdate(float deltaTime) = 0;
};

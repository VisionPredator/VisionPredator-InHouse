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

class IRenderable
{
public:
	virtual void RenderUpdate(float deltaTime) = 0;
};

class IStartable
{
public:
	virtual void Initialize() = 0;
	virtual void Start(uint32_t gameObjectId) = 0;
	virtual void Finish(uint32_t gameObjectId) = 0;
	virtual void Finalize() = 0;
};


#pragma once

class RenderPass
{
protected:
	~RenderPass() = default;

public:
	virtual void Render() = 0;
};


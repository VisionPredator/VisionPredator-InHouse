#pragma once

class Pass
{
public:
	Pass() = default;
	virtual ~Pass() = default;

	virtual void Render() = 0;
};


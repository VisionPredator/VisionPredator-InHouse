#pragma once
namespace Physic
{
class IPhysx
{
public:
	IPhysx() {};
	virtual ~IPhysx() {};

	virtual bool Initialize()  abstract;
	virtual bool Finalize()  abstract;
};

}

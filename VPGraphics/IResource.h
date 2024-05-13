#pragma once
#include "pch.h"

#include "Device.h"



class IResource
{
public:
	IResource();
	IResource(std::shared_ptr<Device> device);
	virtual ~IResource();

	virtual void Release() abstract;

protected:
	std::weak_ptr<Device> m_Device;
};


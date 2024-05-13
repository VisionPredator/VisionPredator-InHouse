#include "pch.h"
#include "IResource.h"

IResource::IResource()
{

}

IResource::IResource(std::shared_ptr<Device> device) : m_Device(device)
{

}

IResource::~IResource()
{

}

void IResource::Release()
{

}

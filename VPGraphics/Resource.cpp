#include "pch.h"
#include "Resource.h"

Resource::Resource()
{

}

Resource::Resource(std::shared_ptr<Device> device) : m_Device(device)
{

}

Resource::~Resource()
{

}

void Resource::Release()
{

}

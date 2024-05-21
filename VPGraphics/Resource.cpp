#include "pch.h"
#include "Resource.h"

Resource::Resource() : m_Device()
{

}

Resource::Resource(std::shared_ptr<Device> device) : m_Device(device)
{

}

Resource::~Resource()
{

}

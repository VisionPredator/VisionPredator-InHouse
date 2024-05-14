#include "pch.h"
#include "Resource.h"

Resource::Resource() : m_Device(nullptr)
{

}

Resource::Resource(Device* device) : m_Device(device)
{

}

Resource::~Resource()
{

}

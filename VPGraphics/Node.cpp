#include "pch.h"
#include "Node.h"

Node::Node() : m_Local(VPMath::Matrix::Identity), m_LocalInverse(VPMath::Matrix::Identity),
m_World(VPMath::Matrix::Identity),m_WorldInverse(VPMath::Matrix::Identity)
{

}

Node::~Node()
{

}

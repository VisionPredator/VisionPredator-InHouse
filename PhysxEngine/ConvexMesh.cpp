#include "pch.h"
#include "ConvexMesh.h"

inline ConvexMesh::~ConvexMesh()
{
	PX_RELEASE(mConvexMesh);
}
#pragma once
#include "Resource.h"

#include <d3d11.h>
#include "Device.h"


/// <summary>
/// index, vertex buffer의 부모 클래스
/// </summary>
class Buffer :
	public Resource
{
public:
	Buffer();
	Buffer(Device* device, UINT count);
	Buffer(Device* device);
	Buffer(Device* device, D3D11_BUFFER_DESC desc);
	Buffer(Device* device, D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA data);
	Buffer(Device* device, D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA data, int count);
	~Buffer();

	ID3D11Buffer* Get() const;
	ID3D11Buffer** GetAddress();
	UINT Count() const;

	virtual void Update() abstract;
	virtual void Release() override;

protected:
	ID3D11Buffer* m_buffer;
	UINT m_count;
	D3D11_BUFFER_DESC m_Desc;
	D3D11_SUBRESOURCE_DATA m_Data;
};



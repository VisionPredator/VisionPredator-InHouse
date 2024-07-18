#pragma once
#include "Resource.h"

#include <d3d11.h>
#include <wrl/client.h>

#include "Device.h"


/// <summary>
/// index, vertex buffer의 부모 클래스
/// </summary>
class Buffer :
	public Resource
{
public:
	Buffer();
	Buffer(std::shared_ptr<Device> device, UINT count);
	Buffer(std::shared_ptr<Device> device);
	Buffer(std::shared_ptr<Device> device, D3D11_BUFFER_DESC desc);
	Buffer(std::shared_ptr<Device> device, D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA data);
	Buffer(std::shared_ptr<Device> device, D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA data, int count);

	Buffer(const std::shared_ptr<Device>& device, const D3D11_BUFFER_DESC* desc, const D3D11_SUBRESOURCE_DATA* data, const int& count);

	~Buffer() override = default;

	ID3D11Buffer* Get() const;
	ID3D11Buffer** GetAddress();
	//Microsoft::WRL::ComPtr<ID3D11Buffer> GetComPtr() { return m_buffer; }
	UINT Count() const;

	virtual void Update() abstract;
	virtual void Release() override;

protected:
	//Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	ID3D11Buffer* m_buffer = nullptr;

	UINT m_count = 0;
	D3D11_BUFFER_DESC m_Desc = {};
	D3D11_SUBRESOURCE_DATA m_Data = {};
};



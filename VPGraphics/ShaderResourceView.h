#pragma once
#include "Resource.h"
#include <string>
#include <wrl/client.h>

/*
 *	Texture �� ���̴� ���ҽ�
 *	�̹��� ������ �ҷ��鿩 �ؽ��� ���ҽ��� ����� ������ �ְų�
 *	RTV�� �����ϴ� �ؽ��ĸ� ����� ������ �ִ´�.
 */

class ShaderResourceView : public Resource
{
public:
	ShaderResourceView(const std::shared_ptr<Device>& device);		// TODO: ����.

	ShaderResourceView(const std::shared_ptr<Device>& device, const std::wstring& filename);
	ShaderResourceView(const std::shared_ptr<Device>& device, const std::shared_ptr<RenderTargetView>& rtv);
	ShaderResourceView(const std::shared_ptr<Device>& device, const std::shared_ptr<Texture2D>& texture2D);

	//ShaderResourceView(std::shared_ptr<Device>device, std::weak_ptr<Texture2D> texture, D3D11_SHADER_RESOURCE_VIEW_DESC desc);
	//ShaderResourceView(std::shared_ptr<Device>device, std::weak_ptr<RenderTargetView> rtv, D3D11_SHADER_RESOURCE_VIEW_DESC desc);
	//ShaderResourceView(std::shared_ptr<Device> device, RenderTargetView* rtv);

	~ShaderResourceView() = default;

	ID3D11ShaderResourceView* Get() const;
	ID3D11ShaderResourceView** GetAddress();
	void Release() override;

	UINT GetWidth() const { return m_Width; }
	UINT GetHeight() const { return m_Height; }

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;

	UINT m_Width = 0;
	UINT m_Height = 0;
};


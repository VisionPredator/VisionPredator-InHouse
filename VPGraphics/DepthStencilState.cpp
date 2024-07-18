#include "pch.h"
#include "DepthStencilState.h"

#include "Defines.h"

DepthStencilState::DepthStencilState(const std::shared_ptr<Device>& device, const DepthStencilStateType& type) : Resource(device)
{
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	switch (type)
	{
	case DepthStencilStateType::Default:
		{
			desc.DepthEnable = true;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			desc.DepthFunc = D3D11_COMPARISON_LESS;
			desc.StencilEnable = false;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.BackFace = desc.FrontFace;
		break;
		}
	case DepthStencilStateType::DisableDepth:
		{
			desc.DepthEnable = false;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			desc.DepthFunc = D3D11_COMPARISON_LESS;
			desc.StencilEnable = false;
		break;
		}
	case DepthStencilStateType::NoDepthWrites:
		{
			desc.DepthEnable = true;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			desc.DepthFunc = D3D11_COMPARISON_LESS;
			desc.StencilEnable = false;
		break;
		}

	}

	HR_CHECK(device->Get()->CreateDepthStencilState(&desc, m_State.GetAddressOf()));
}

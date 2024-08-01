#pragma once

#include <d3d11.h>
#include <wrl/client.h>



class D3DUtill
{
public:
	static float RandF();
	static float RandF(float a, float b);
	VPMath::Vector4 RandUnitVec3(VPMath::Vector4 n);

	///<summary>For simple random data or noise generation</summary>
	static void CreateRandomTexture1DSRV(ID3D11Device* device, ID3D11ShaderResourceView** ppSRV);
	///<summary>A complex resource graphic effect for particle system</summary>
	static void CreateRandomTexture2DSRV(ID3D11Device* device, ID3D11ShaderResourceView** ppSRV);

	static void CreateTexture2DArraySRV(
		ID3D11Device* device, ID3D11DeviceContext* context,
		ID3D11ShaderResourceView** ppSRV, const std::vector<std::wstring>& filename);
};


#pragma once
#include "Resource.h"

class Texture2D : public Resource
{
public:
	Texture2D(std::shared_ptr<Device> device, D3D11_TEXTURE2D_DESC desc);
	Texture2D(std::shared_ptr<Device> device);
	~Texture2D();


	virtual void Release() override;

	ID3D11Texture2D* Get();
	ID3D11Texture2D** GetAddress();



private:
	ID3D11Texture2D* m_tex;
	D3D11_TEXTURE2D_DESC m_desc;
};


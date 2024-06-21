#include "pch.h"
#include "DebugDrawManager.h"
#include "Device.h"
#include "Defines.h"
#include "Camera.h"

void DebugDrawManager::Initialize(const std::shared_ptr<Device>& device)
{
    m_States = std::make_unique<CommonStates>(device->Get());
    m_Batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(device->Context());
    m_BatchEffect = std::make_unique<BasicEffect>(device->Get());
    m_BatchEffect->SetVertexColorEnabled(true);

    void const* shaderByteCode;
    size_t byteCodeLength;

    m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

    HR_CHECK(device->Get()->CreateInputLayout(VertexPositionColor::InputElements,
        VertexPositionColor::InputElementCount,
        shaderByteCode, byteCodeLength,
        &m_BatchInputLayout));
}

void DebugDrawManager::Execute(const std::shared_ptr<Device>& device, const std::shared_ptr<Camera>& camera)
{
    device->Context()->OMSetBlendState(m_States->Opaque(), nullptr, 0xFFFFFFFF);
    device->Context()->OMSetDepthStencilState(m_States->DepthDefault(), 0);
    device->Context()->RSSetState(m_States->CullNone());

    m_BatchEffect->Apply(device->Context());
    m_BatchEffect->SetView(camera->View());
    m_BatchEffect->SetProjection(camera->Proj());

    device->Context()->IASetInputLayout(m_BatchInputLayout.Get());

    m_Batch->Begin();

    while (!m_SphereInfos.empty())
    {
        Draw(m_SphereInfos.front());
        m_SphereInfos.pop();
    }

    m_Batch->End();

    device->Context()->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
    device->Context()->OMSetDepthStencilState(nullptr, 0);
    device->Context()->RSSetState(nullptr);
}

void DebugDrawManager::Draw(const debug::SphereInfo& info)
{
    SimpleMath::Vector3 origin = info.Sphere.Center;

    const float radius = info.Sphere.Radius;

    SimpleMath::Vector3 xaxis = SimpleMath::Vector3::UnitX * radius;
    SimpleMath::Vector3 yaxis = SimpleMath::Vector3::UnitY * radius;
    SimpleMath::Vector3 zaxis = SimpleMath::Vector3::UnitZ * radius;

    debug::RingInfo ringInfo;
    ringInfo.Origin = origin;
    ringInfo.Color = info.Color;

    ringInfo.MajorAxis = xaxis;
    ringInfo.MinorAxis = zaxis;
    DrawRing(ringInfo);

    ringInfo.MajorAxis = xaxis;
    ringInfo.MinorAxis = yaxis;
    DrawRing(ringInfo);

    ringInfo.MajorAxis = yaxis;
    ringInfo.MinorAxis = zaxis;
    DrawRing(ringInfo);
}

void DebugDrawManager::DrawRing(const debug::RingInfo& info)
{
    static const size_t c_ringSegments = 32;

    VertexPositionColor verts[c_ringSegments + 1];

    FLOAT fAngleDelta = XM_2PI / float(c_ringSegments);
    // Instead of calling cos/sin for each segment we calculate
    // the sign of the angle delta and then incrementally calculate sin
    // and cosine from then on.
    XMVECTOR cosDelta = XMVectorReplicate(cosf(fAngleDelta));
    XMVECTOR sinDelta = XMVectorReplicate(sinf(fAngleDelta));
    XMVECTOR incrementalSin = XMVectorZero();
    static const XMVECTORF32 s_initialCos =
    {
        1.f, 1.f, 1.f, 1.f
    };
    XMVECTOR incrementalCos = s_initialCos.v;
    for (size_t i = 0; i < c_ringSegments; i++)
    {
        XMVECTOR pos = XMVectorMultiplyAdd(info.MajorAxis, incrementalCos, info.Origin);
        pos = XMVectorMultiplyAdd(info.MinorAxis, incrementalSin, pos);
        XMStoreFloat3(&verts[i].position, pos);
        XMStoreFloat4(&verts[i].color, info.Color);
        // Standard formula to rotate a vector.
        XMVECTOR newCos = incrementalCos * cosDelta - incrementalSin * sinDelta;
        XMVECTOR newSin = incrementalCos * sinDelta + incrementalSin * cosDelta;
        incrementalCos = newCos;
        incrementalSin = newSin;
    }
    verts[c_ringSegments] = verts[0];

    m_Batch->Draw(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, verts, c_ringSegments + 1);
}

void DebugDrawManager::DrawCube(const SimpleMath::Matrix& worldTransform, const SimpleMath::Color& color)
{
	static const XMVECTORF32 s_verts[8] =
	{
		{ { { -1.f, -1.f, -1.f, 0.f } } },
		{ { {  1.f, -1.f, -1.f, 0.f } } },
		{ { {  1.f, -1.f,  1.f, 0.f } } },
		{ { { -1.f, -1.f,  1.f, 0.f } } },
		{ { { -1.f,  1.f, -1.f, 0.f } } },
		{ { {  1.f,  1.f, -1.f, 0.f } } },
		{ { {  1.f,  1.f,  1.f, 0.f } } },
		{ { { -1.f,  1.f,  1.f, 0.f } } }
	};

    static const WORD s_indices[] =
    {
        0, 1,
        1, 2,
        2, 3,
        3, 0,
        4, 5,
        5, 6,
        6, 7,
        7, 4,
        0, 4,
        1, 5,
        2, 6,
        3, 7
    };

    VertexPositionColor verts[8];
    for (size_t i = 0; i < 8; ++i)
    {
        XMVECTOR v = XMVector3Transform(s_verts[i], worldTransform);
        XMStoreFloat3(&verts[i].position, v);
        XMStoreFloat4(&verts[i].color, color);
    }

    m_Batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_LINELIST, s_indices, static_cast<UINT>(std::size(s_indices)), verts, 8);
}


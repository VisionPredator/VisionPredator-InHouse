#include "pch.h"
#include "Camera.h"

#include <windowsx.h>


Camera::Camera() : m_pos(0, 5, -25), m_forward(0, 0, 1), m_up(0, 1, 0), m_right(1, 0, 0), m_nearZ(0), m_farZ(0), m_FOV(0), m_nearWindowHeight(0), m_farWindowHeight(0), m_ratio(1),
m_worldMT(Matrix::Identity), m_viewMT(Matrix::Identity), m_projMT(Matrix::Identity), m_worldviewprojMT(Matrix::Identity),
m_world(), m_view(), m_proj(), m_worldviewproj(), m_moveSpeed(1)
{

}

Camera::~Camera()
{

}

void Camera::Initialize(double ratio)
{
	m_ratio = static_cast<float>(ratio);
	m_worldMT = Matrix::Identity;
	m_viewMT = Matrix::Identity;
	m_projMT = Matrix::Identity;
	m_worldviewprojMT = Matrix::Identity;

	//XMStoreFloat4x4(&m_world, m_worldMT);
	//XMStoreFloat4x4(&m_view, m_viewMT);
	//XMStoreFloat4x4(&m_proj, m_projMT);
	//XMStoreFloat4x4(&m_worldviewproj, m_projMT);

	m_world = m_worldMT;
	m_view = m_viewMT;
	m_proj = m_projMT;
	m_worldviewproj = m_projMT;
	

	m_FOV = 0.25f * 3.14f;
	m_nearZ = 1.f;
	m_farZ = 1000;

	m_nearWindowHeight = 2 * m_nearZ * tanf(0.5f * m_FOV);
	m_farWindowHeight = 2 * m_farZ * tanf(0.5f * m_FOV);

	m_proj = DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_ratio, m_nearZ, m_farZ);
	//m_projMT = DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_ratio, m_nearZ, m_farZ);
	//XMStoreFloat4x4(&m_proj, m_projMT);
}

void Camera::Update(double dt)
{
	UpdateView();

	int speed = 10;
	//w
	if (GetAsyncKeyState(0x57))
	{
		Walk(dt * speed);
	}
	//s
	if (GetAsyncKeyState(0x53))
	{
		Walk(dt * -speed);
	}
	//a
	if (GetAsyncKeyState(0x41))
	{
		Strafe(dt * -speed);
	}
	//d
	if (GetAsyncKeyState(0x44))
	{
		Strafe(dt * speed);
	}
	//q
	if (GetAsyncKeyState(0x51))
	{
		RotateY(-dt);
	}
	//e
	if (GetAsyncKeyState(0x45))
	{
		RotateY(dt);
	}




	m_worldviewprojMT = m_worldMT * m_viewMT * m_projMT;
}

Vector3 Camera::GetPosition() const
{
	return m_pos;
}

Matrix Camera::World() const
{
	return m_world;
}

Matrix Camera::View() const
{
	return m_view;
}


Matrix Camera::Proj() const
{
	return m_proj;
}

void Camera::Walk(float d)
{
	//DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d);
	//DirectX::XMVECTOR f = DirectX::XMLoadFloat3(&m_forward);
	//DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&m_pos);
	//XMStoreFloat3(&m_pos, DirectX::XMVectorMultiplyAdd(s, f, p));

	Vector3 s = Vector3(d, d, d);
	Vector3 f = m_forward;
	Vector3 p = m_pos;

	m_pos = s * f + p;
}

void Camera::UpDown(float d)
{
	DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d);
	DirectX::XMVECTOR u = DirectX::XMLoadFloat3(&m_up);
	DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&m_pos);
	XMStoreFloat3(&m_pos, DirectX::XMVectorMultiplyAdd(s, u, p));
}


void Camera::Strafe(float d)
{
	DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d);
	DirectX::XMVECTOR r = DirectX::XMLoadFloat3(&m_right);
	DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&m_pos);
	XMStoreFloat3(&m_pos, DirectX::XMVectorMultiplyAdd(s, r, p));
}

void Camera::Pitch(float angle)
{
	DirectX::XMMATRIX r = DirectX::XMMatrixRotationAxis(XMLoadFloat3(&m_right), angle);

	XMStoreFloat3(&m_right, DirectX::XMVector3TransformNormal(XMLoadFloat3(&m_right), r));
	XMStoreFloat3(&m_up, DirectX::XMVector3TransformNormal(XMLoadFloat3(&m_up), r));
	XMStoreFloat3(&m_forward, DirectX::XMVector3TransformNormal(XMLoadFloat3(&m_forward), r));
}


void Camera::Roll(float angle)
{
	DirectX::XMMATRIX r = DirectX::XMMatrixRotationAxis(XMLoadFloat3(&m_forward), angle);

	XMStoreFloat3(&m_right, DirectX::XMVector3TransformNormal(XMLoadFloat3(&m_right), r));
	XMStoreFloat3(&m_up, DirectX::XMVector3TransformNormal(XMLoadFloat3(&m_up), r));
	XMStoreFloat3(&m_forward, DirectX::XMVector3TransformNormal(XMLoadFloat3(&m_forward), r));
}


void Camera::Yaw(float angle)
{
	DirectX::XMMATRIX r = DirectX::XMMatrixRotationAxis(XMLoadFloat3(&m_up), angle);

	XMStoreFloat3(&m_right, DirectX::XMVector3TransformNormal(XMLoadFloat3(&m_right), r));
	XMStoreFloat3(&m_up, DirectX::XMVector3TransformNormal(XMLoadFloat3(&m_up), r));
	XMStoreFloat3(&m_forward, DirectX::XMVector3TransformNormal(XMLoadFloat3(&m_forward), r));
}

void Camera::RotateX(float angle)
{
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationX(angle);

	XMStoreFloat3(&m_right, XMVector3TransformNormal(XMLoadFloat3(&m_right), R));
	XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), R));
	XMStoreFloat3(&m_forward, XMVector3TransformNormal(XMLoadFloat3(&m_forward), R));
}

void Camera::RotateY(float angle)
{
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationY(angle);

	XMStoreFloat3(&m_right, XMVector3TransformNormal(XMLoadFloat3(&m_right), R));
	XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), R));
	XMStoreFloat3(&m_forward, XMVector3TransformNormal(XMLoadFloat3(&m_forward), R));
}

void Camera::RotateZ(float angle)
{
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationZ(angle);

	XMStoreFloat3(&m_right, XMVector3TransformNormal(XMLoadFloat3(&m_right), R));
	XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), R));
	XMStoreFloat3(&m_forward, XMVector3TransformNormal(XMLoadFloat3(&m_forward), R));
}

DirectX::XMFLOAT4X4 Camera::WorldViewProj()
{
	XMStoreFloat4x4(&m_worldviewproj, m_worldviewprojMT);
	return m_worldviewproj;
}

void Camera::SetPos(DirectX::XMFLOAT3 pos)
{
	m_pos = pos;
}

void Camera::OnResize(double ratio)
{
	m_ratio = static_cast<float>(ratio);
	XMStoreFloat4x4(&m_worldviewproj, m_projMT);

	m_projMT = DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_ratio, m_nearZ, m_farZ);
	XMStoreFloat4x4(&m_proj, m_projMT);
}

void Camera::UpdateView()
{
	DirectX::XMVECTOR R = DirectX::XMLoadFloat3(&m_right);
	DirectX::XMVECTOR U = DirectX::XMLoadFloat3(&m_up);
	DirectX::XMVECTOR F = DirectX::XMLoadFloat3(&m_forward);
	DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&m_pos);

	F = DirectX::XMVector3Normalize(F);
	U = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(F, R));
	R = DirectX::XMVector3Cross(U, F);

	float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, R));
	float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, U));
	float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, F));

	DirectX::XMStoreFloat3(&m_right, R);
	DirectX::XMStoreFloat3(&m_forward, F);
	DirectX::XMStoreFloat3(&m_up, U);

	m_view._11 = m_right.x;
	m_view._21 = m_right.y;
	m_view._31 = m_right.z;
	m_view._41 = x;

	m_view._12 = m_up.x;
	m_view._22 = m_up.y;
	m_view._32 = m_up.z;
	m_view._42 = y;

	m_view._13 = m_forward.x;
	m_view._23 = m_forward.y;
	m_view._33 = m_forward.z;
	m_view._43 = z;


	m_view._14 = 0.0f;
	m_view._24 = 0.0f;
	m_view._34 = 0.0f;
	m_view._44 = 1.0f;

	m_viewMT = XMLoadFloat4x4(&m_view);
}


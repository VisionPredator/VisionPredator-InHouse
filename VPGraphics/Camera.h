#pragma once

using namespace DirectX::SimpleMath;

class Camera
{
public:
	Camera();
	~Camera();

	void Initialize(double ratio);
	void Update(double dt);
	Vector3 GetPosition() const;

	Matrix World() const;
	Matrix View() const;
	Matrix Proj() const;

	void Strafe(float d);
	void Walk(float d);
	void UpDown(float d);

	void Pitch(float angle); //x rot
	void Yaw(float angle);	//y rot
	void Roll(float angle);	//z rot

	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);

	DirectX::XMFLOAT4X4 WorldViewProj();

	void SetPos(DirectX::XMFLOAT3 pos);
	void OnResize(double ratio);
private:
	void UpdateView();

private:

	//DirectX::XMFLOAT3 m_pos;		//카메라 위치
	//DirectX::XMFLOAT3 m_forward;	//카메라의 전방 벡터
	//DirectX::XMFLOAT3 m_up;			//카메라의 업 벡터
	//DirectX::XMFLOAT3 m_right;		//카메라의 오른쪽벡터

	Vector3 m_pos;		//카메라 위치
	Vector3 m_forward;	//카메라의 전방 벡터
	Vector3 m_up;			//카메라의 업 벡터
	Vector3 m_right;		//카메라의 오른쪽벡터

	float m_nearZ;
	float m_farZ;

	float m_FOV;

	float m_nearWindowHeight;
	float m_farWindowHeight;
	float m_ratio;

	//DirectX::XMMATRIX m_worldMT;
	//DirectX::XMMATRIX m_viewMT;
	//DirectX::XMMATRIX m_projMT;
	//DirectX::XMMATRIX m_worldviewprojMT;

	Matrix m_worldMT;
	Matrix m_viewMT;
	Matrix m_projMT;
	Matrix m_worldviewprojMT;

	Matrix m_world;
	Matrix m_view;
	Matrix m_proj;
	Matrix m_worldviewproj;

	//DirectX::XMFLOAT4X4 m_world;
	//DirectX::XMFLOAT4X4 m_view;
	//DirectX::XMFLOAT4X4 m_proj;
	//DirectX::XMFLOAT4X4 m_worldviewproj;

private:
	float m_moveSpeed;
};


#pragma once
class Camera
{
public:
	Camera();
	~Camera();

	void Initialize(double ratio);
	void Update(double dt);
	DirectX::XMFLOAT3 GetPosition() const;

	DirectX::XMFLOAT4X4 World() const;
	DirectX::XMFLOAT4X4 View() const;
	DirectX::XMFLOAT4X4 Proj() const;

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

	DirectX::XMFLOAT3 m_pos;		//ī�޶� ��ġ
	DirectX::XMFLOAT3 m_forward;	//ī�޶��� ���� ����
	DirectX::XMFLOAT3 m_up;			//ī�޶��� �� ����
	DirectX::XMFLOAT3 m_right;		//ī�޶��� �����ʺ���


	float m_nearZ;
	float m_farZ;

	float m_FOV;

	float m_nearWindowHeight;
	float m_farWindowHeight;
	float m_ratio;

	DirectX::XMMATRIX m_worldMT;
	DirectX::XMMATRIX m_viewMT;
	DirectX::XMMATRIX m_projMT;
	DirectX::XMMATRIX m_worldviewprojMT;

	DirectX::XMFLOAT4X4 m_world;
	DirectX::XMFLOAT4X4 m_view;
	DirectX::XMFLOAT4X4 m_proj;
	DirectX::XMFLOAT4X4 m_worldviewproj;

private:
	float m_moveSpeed;
};

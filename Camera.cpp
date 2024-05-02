#include "Camera.h"


Camera::~Camera()
{
	std::cout << "camera des" << std::endl;
}

Camera::Camera()
{
	std::cout << "camera cons" << std::endl;
}

DirectX::XMFLOAT4X4 Camera::GetView4x4f() const
{
	assert(!mViewDirty);
	return mView;
}

DirectX::XMFLOAT4X4 Camera::GetProj4x4f() const
{
	return mProj;
}

DirectX::XMMATRIX Camera::GetView()const
{
	assert(!mViewDirty);
	return XMLoadFloat4x4(&mView);
}

DirectX::XMMATRIX Camera::GetProj()const
{
	return XMLoadFloat4x4(&mProj);
}

void Camera::LookAtLH(const DirectX::XMFLOAT3& pos,
	const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up)
{
	DirectX::XMVECTOR P = XMLoadFloat3(&pos);
	DirectX::XMVECTOR T = XMLoadFloat3(&target);
	DirectX::XMVECTOR U = XMLoadFloat3(&up);

	XMStoreFloat4x4(&mView, DirectX::XMMatrixLookAtLH(P, T, U));
}

void Camera::OffCenterLH(float ViewLeft, float ViewRight,
	 float ViewBottom, float ViewTop, float NearZ, float FarZ)
{
	XMStoreFloat4x4(&mProj, DirectX::XMMatrixOrthographicOffCenterLH(ViewLeft, ViewRight,
		ViewBottom, ViewTop, NearZ, FarZ));
}

void Camera::UpdateViewMatrix()
{
	if (mViewDirty)
	{
		mViewDirty = false;
	}
}
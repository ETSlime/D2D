#pragma once

class Values
{
public:
	const static float PI;

	const static DirectX::XMFLOAT3 ZeroVec3;
	const static DirectX::XMFLOAT3 OneVec3;
	const static DirectX::XMFLOAT3 LeftVec;
	const static DirectX::XMFLOAT3 RightVec;
	const static DirectX::XMFLOAT3 UpVec;
	const static DirectX::XMFLOAT3 DownVec;
	const static DirectX::XMFLOAT3 FwdVec;
	const static DirectX::XMFLOAT3 ScreenSize;

	const static DirectX::XMFLOAT2 ZeroVec2;
	const static DirectX::XMFLOAT2 OneVec2;
	const static DirectX::XMFLOAT2 uvLT;
	const static DirectX::XMFLOAT2 uvLB;
	const static DirectX::XMFLOAT2 uvRT;
	const static DirectX::XMFLOAT2 uvRB;

	const static DirectX::XMFLOAT4 Red;
	const static DirectX::XMFLOAT4 Green;
	const static DirectX::XMFLOAT4 Blue;
	const static DirectX::XMFLOAT4 Magenta;
	const static DirectX::XMFLOAT4 Black;
	const static DirectX::XMFLOAT4 White;
	const static DirectX::XMFLOAT4 ClearColor;

	const static DirectX::XMFLOAT4X4 Identity;
};

class DXMath
{
public:
	static DirectX::XMFLOAT4X4 Translation(const DirectX::XMFLOAT3& position)
	{
		DirectX::XMFLOAT4X4 m;
		XMStoreFloat4x4(&m,DirectX::XMMatrixTranslation(position.x, position.y, position.z));
		return m;
	}

	static DirectX::XMFLOAT4X4 Translation(const float& x, const float& y, const float& z)
	{
		return Translation(DirectX::XMFLOAT3(x, y, z));
	}

	static DirectX::XMFLOAT4X4 TranslationX(const float& x)
	{
		return Translation(x, 0, 0);
	}

	static DirectX::XMFLOAT4X4 TranslationY(const float& y)
	{
		return Translation(0, y, 0);
	}

	static DirectX::XMFLOAT4X4 TranslationZ(const float& z)
	{
		return Translation(0, 0, z);
	}

	static DirectX::XMFLOAT4X4 Scaling(const DirectX::XMFLOAT3& size)
	{
		DirectX::XMFLOAT4X4 m;
		XMStoreFloat4x4(&m, DirectX::XMMatrixScaling(size.x, size.y, size.z));
		return m;
	}

	static DirectX::XMFLOAT4X4 Scaling(const float& x, const float& y, const float& z)
	{
		DirectX::XMFLOAT4X4 m;
		XMStoreFloat4x4(&m, DirectX::XMMatrixScaling(x, y, z));
		return m;
	}

	static DirectX::XMFLOAT4X4 ScalingX(const float& x)
	{
		return Scaling(x, 1, 1);
	}

	static DirectX::XMFLOAT4X4 ScalingY(const float& y)
	{
		return Scaling(1, y, 1);
	}

	static DirectX::XMFLOAT4X4 ScalingZ(const float& z)
	{
		return Scaling(1, 1, z);
	}

	static DirectX::XMFLOAT4X4 RotationInDegree(const float& rotation)
	{
		DirectX::XMFLOAT4X4 m;
		XMStoreFloat4x4(&m, DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation)));
		return m;
	}

	static DirectX::XMFLOAT4X4 RotationInRadian(const float& rotation)
	{
		DirectX::XMFLOAT4X4 m;
		XMStoreFloat4x4(&m, DirectX::XMMatrixRotationZ(rotation));
		return m;
	}
};
#pragma once

#include "Util.h"
#include "VertexTypes.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "DefaultBuffer.h"
#include "UploadBuffer.h"
#include "Timer.h"
#include "MagicTowerApp.h"


class IRectBase
{
public:

	virtual void SetVertices() = 0;
	virtual void SetIndices() = 0;
	virtual void Render() = 0;
	virtual void Update() = 0;

	template<typename VertexType>
	void CreateRenderResource(const std::vector<VertexType>vertices,
		const std::vector<UINT>indices, std::wstring shaderPath);

	void SetShader(std::wstring shaderPath);


	int indexCount = 0;
	DefaultBuffer* mVertexBuffer = nullptr;
	DefaultBuffer* mIndexBuffer = nullptr;
	VertexShader* mVertexShader = nullptr;
	PixelShader* mPixelShader = nullptr;
	InputLayout* mInputLayout = nullptr;

	UploadBuffer<DirectX::XMFLOAT4X4>* mWorldBuffer = nullptr;
	
	ID3D11Device* mDevice = MagicTowerApp::get_instance().mDevice;
	ID3D11DeviceContext* mDeviceContext = MagicTowerApp::get_instance().mDeviceContext;
};

template<typename VertexType>
class RectBase : public IRectBase
{
public:
	RectBase();
	virtual ~RectBase();

	virtual void Render() override;

	void CreateRenderResource(const std::vector<VertexType>vertices,
		const std::vector<UINT>indices, std::wstring shaderPath);
};

template <>
class RectBase<VertexTexture>:public IRectBase
{
public:
	RectBase(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation);
	virtual ~RectBase();

	void CreateRenderResource(const std::vector<VertexTexture>vertices,
		const std::vector<UINT>indices, std::wstring shaderPath);
	virtual void Render() override;

	void UpdateWorld();
	void UpdatePosition(DirectX::XMFLOAT3 newPosition);
	void UpdateSize(DirectX::XMFLOAT3 newScale);
	void UpdateRotation(float newRotation);
	
protected:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 size;
	float rotation;
	ID3D11ShaderResourceView* srv = nullptr;
};




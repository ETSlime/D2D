#include "UITextureRect.h"

UITextureRect::UITextureRect(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
	DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation, std::wstring path)
	:mDevice(device), mDeviceContext(deviceContext), position(position), size(size), rotation(rotation)
{
	//vertices
	{
		vertices.assign(4, VertexTexture());

		vertices[0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.0f);
		vertices[0].uv = DirectX::XMFLOAT2(0, 1);

		vertices[1].position = DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);
		vertices[1].uv = DirectX::XMFLOAT2(1, 0);

		vertices[2].position = DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f);
		vertices[2].uv = DirectX::XMFLOAT2(1, 1);

		vertices[3].position = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f);
		vertices[3].uv = DirectX::XMFLOAT2(0, 0);
	}

	//VertexBuffer
	mVertexBuffer = new DefaultBuffer();
	mVertexBuffer->CreateDefaultBuffer(device, deviceContext, vertices.data(),
		vertices.size(), sizeof(VertexTexture), D3D11_USAGE_DYNAMIC);

	//IndexBuffer
	indices = { 0, 1, 2, 0, 3, 1 };
	mIndexBuffer = new DefaultBuffer();
	mIndexBuffer->CreateDefaultBuffer(device, deviceContext, indices.data(),
		indices.size(), sizeof(UINT), D3D11_USAGE_DYNAMIC);

	//VertexShader
	mVertexShader = new VertexShader(mDevice, mDeviceContext);
	mVertexShader->Create(ShaderPath + L"UITexture.hlsl", "VS");

	//PixelShader
	mPixelShader = new PixelShader(mDevice, mDeviceContext);
	mPixelShader->Create(ShaderPath + L"UITexture.hlsl", "PS");

	//InputLayout
	mInputLayout = new InputLayout(mDevice, mDeviceContext);
	mInputLayout->Create(VertexTexture::descs, VertexTexture::count, mVertexShader->GetBlob());

	//WorldBuffer
	mWorldBuffer = new UploadBuffer<DirectX::XMFLOAT4X4>(
		device, deviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);




	//SRV
	{
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile
		(
			mDevice,
			path.c_str(),
			nullptr,
			nullptr,
			&srv,
			nullptr
		);
		Assert(SUCCEEDED(hr));
	}

	mShaderBuffer =  new UploadBuffer<Data>(
		device, deviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);
}

UITextureRect::UITextureRect(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
	DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation)
	:mDevice(device), mDeviceContext(deviceContext), position(position), size(size), rotation(rotation)
{
	//vertices
	{
		vertices.assign(4, VertexTexture());

		vertices[0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.0f);
		vertices[0].uv = DirectX::XMFLOAT2(0, 1);

		vertices[1].position = DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f);
		vertices[1].uv = DirectX::XMFLOAT2(1, 0);

		vertices[2].position = DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f);
		vertices[2].uv = DirectX::XMFLOAT2(1, 1);

		vertices[3].position = DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f);
		vertices[3].uv = DirectX::XMFLOAT2(0, 0);
	}

	//VertexBuffer
	mVertexBuffer = new DefaultBuffer();
	mVertexBuffer->CreateDefaultBuffer(device, deviceContext, vertices.data(),
		vertices.size(), sizeof(VertexTexture), D3D11_USAGE_DYNAMIC);

	//IndexBuffer
	indices = { 0, 1, 2, 0, 3, 1 };
	mIndexBuffer = new DefaultBuffer();
	mIndexBuffer->CreateDefaultBuffer(device, deviceContext, indices.data(),
		indices.size(), sizeof(UINT), D3D11_USAGE_IMMUTABLE);

	//VertexShader
	mVertexShader = new VertexShader(mDevice, mDeviceContext);
	mVertexShader->Create(ShaderPath + L"UITexture.hlsl", "VS");

	//PixelShader
	mPixelShader = new PixelShader(mDevice, mDeviceContext);
	mPixelShader->Create(ShaderPath + L"UITexture.hlsl", "PS");

	//InputLayout
	mInputLayout = new InputLayout(mDevice, mDeviceContext);
	mInputLayout->Create(VertexTexture::descs, VertexTexture::count, mVertexShader->GetBlob());

	//WorldBuffer
	mWorldBuffer = new UploadBuffer<DirectX::XMFLOAT4X4>(
		device, deviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);

}

UITextureRect::~UITextureRect()
{
	SAFE_DELETE(mVertexBuffer);
	SAFE_DELETE(mIndexBuffer);
	SAFE_DELETE(mVertexShader);
	SAFE_DELETE(mPixelShader);
	SAFE_DELETE(mInputLayout);
	SAFE_DELETE(mWorldBuffer);
	SafeRelease(&srv);

}

void UITextureRect::SetShader(std::wstring shaderPath)
{
	mVertexShader->Clear();
	mPixelShader->Clear();

	mVertexShader->Create(shaderPath, "VS");
	mPixelShader->Create(shaderPath, "PS");
}

void UITextureRect::Update()
{
	UpdateWorld();
	Move();
}

void UITextureRect::UpdateWorld()
{
	//D3DXMatrixTranslation(&T, position.x, position.y, position.z);
	//D3DXMatrixScaling(&S, size.x, size.y, size.z);
	//D3DXMatrixRotationZ(&R, (float)D3DXToRadian(rotation));
	DirectX::XMMATRIX S, R, T, W;
	T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	S = DirectX::XMMatrixScaling(size.x, size.y, size.z);
	R = DirectX::XMMatrixRotationZ((float)D3DXToRadian(rotation));

	W = S * R * T;
	XMStoreFloat4x4(&world, W);
	//wb->SetWorld(world);
}

void UITextureRect::Render()
{
	//vb->SetIA();
	//ib->SetIA();
	//il->SetIA();
	//DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//vs->SetShader();
	//ps->SetShader();
	//wb->SetVSBuffer(0);
	//sb->SetPSBuffer(0);
	//DC->PSSetShaderResources(0, 1, &srv);

	//DC->DrawIndexed(ib->GetCount(), 0, 0);

	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer->GetBuffer(), mVertexBuffer->GetStride(), &offset);
	mDeviceContext->IASetIndexBuffer(mIndexBuffer->GetResource(), DXGI_FORMAT_R32_UINT, offset);
	//mInputLayout->SetIA();
	mDeviceContext->IASetInputLayout(mInputLayout->GetInputLayout());

	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mVertexShader->SetShader();
	mPixelShader->SetShader();

	//mWorldBuffer->SetVSBuffer(0);
	mWorldBuffer->MapData(mDeviceContext, world);
	//sb->SetPSBuffer(0);
	mShaderBuffer->MapData(mDeviceContext, shaderData);
	mDeviceContext->VSSetConstantBuffers(0, 1, mWorldBuffer->Resource());

	mDeviceContext->PSSetShaderResources(0, 1, &srv);

	mDeviceContext->DrawIndexed(indices.size(), 0, 0);
}

void UITextureRect::changeshaded(int i)
{

	//*(this->sb->GetSelectionPtr()) = i;
	shaderData.Selection = i;

}

void UITextureRect::Move()
{

}

void UITextureRect::GUI()
{
}

std::string UITextureRect::MakeLabel(std::string preFix)
{
	return std::string();
}

void UITextureRect::MapVertexBuffer()
{
	mDeviceContext->Map(mVertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
}

void UITextureRect::UnmapVertexBuffer()
{
	memcpy(subResource.pData, vertices.data(), sizeof(vertices[0]) * vertices.size());
	mDeviceContext->Unmap(mVertexBuffer->GetResource(), 0);

}

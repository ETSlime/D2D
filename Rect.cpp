#include "Rect.h"

Rect::Rect(ID3D11Device* device, ID3D11DeviceContext* deviceContext, 
	DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation)
	:mDevice(device),mDeviceContext(deviceContext), position(position), size(size), rotation(rotation)
{
	//vertices
	{
		vertices.assign(4, VertexColor());
		{
			vertices[0].position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			vertices[0].color = DirectX::XMFLOAT4(1, 0, 0, 1);

			vertices[1].position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertices[1].color = DirectX::XMFLOAT4(1, 0, 0, 1);

			vertices[2].position = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
			vertices[2].color = DirectX::XMFLOAT4(1, 0, 0, 1);

			vertices[3].position = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
			vertices[3].color = DirectX::XMFLOAT4(1, 0, 0, 1);
		}

	}

	//vertexBuffer
	{
		mVertexBuffer = new DefaultBuffer();
		mVertexBuffer->CreateDefaultBuffer(device, deviceContext, vertices.data(),
			vertices.size(), sizeof(VertexTexture), D3D11_USAGE_DYNAMIC);
	}

	//indexBuffer
	{
		indices = { 0, 1, 2, 2, 1, 3 };

		mIndexBuffer = new DefaultBuffer();
		mIndexBuffer->CreateDefaultBuffer(device, deviceContext, indices.data(),
			indices.size(), sizeof(UINT), D3D11_USAGE_IMMUTABLE);
	}

	//vertexShader
	{
		mVertexShader = new VertexShader(mDevice, mDeviceContext);
		mVertexShader->Create(ShaderPath + L"VertexColor.hlsl", "VS");
	}

	//inputLayout
	{
		mInputLayout = new InputLayout(mDevice, mDeviceContext);
		mInputLayout->Create(VertexColor::descs, VertexColor::count, mVertexShader->GetBlob());
	}

	//pixelShader
	{
		mPixelShader = new PixelShader(mDevice, mDeviceContext);
		mPixelShader->Create(ShaderPath + L"VertexColor.hlsl", "PS");
	}

	//world
	{
		mWorldBuffer = new UploadBuffer<DirectX::XMFLOAT4X4>(
			mDevice, mDeviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);
	}

	edge = new RectEdge();
	box = new BoundingBox(edge);
}

Rect::~Rect()
{

	SAFE_DELETE(edge);
	SAFE_DELETE(box);

	SAFE_DELETE(mWorldBuffer);

	SAFE_DELETE(mInputLayout);

	SAFE_DELETE(mVertexShader);
	SAFE_DELETE(mPixelShader);

	SAFE_DELETE(mIndexBuffer);
	SAFE_DELETE(mVertexBuffer);
}

void Rect::Update()
{
	UpdateWorld();
}

void Rect::Render()
{
	//vb->SetIA();
	//ib->SetIA();
	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer->GetBuffer(), mVertexBuffer->GetStride(), &offset);
	mDeviceContext->IASetIndexBuffer(mIndexBuffer->GetResource(), DXGI_FORMAT_R32_UINT, offset);

	//il->SetIA();
	mDeviceContext->IASetInputLayout(mInputLayout->GetInputLayout());

	//DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//vs->SetShader();
	//ps->SetShader();
	mVertexShader->SetShader();
	mPixelShader->SetShader();
	
	//wb->SetVSBuffer(0);
	mWorldBuffer->MapData(mDeviceContext, world);
	mDeviceContext->VSSetConstantBuffers(0, 1, mWorldBuffer->Resource());

	mDeviceContext->DrawIndexed(indices.size(), 0, 0);
}

void Rect::Move()
{
	//if (Keyboard::Get()->Press('W'))
	//	position.y += 100 * Time::Delta();
	//else if (Keyboard::Get()->Press('S'))
	//	position.y -= 100 * Time::Delta();

	//if (Keyboard::Get()->Press('A'))
	//	position.x -= 100 * Time::Delta();
	//else if (Keyboard::Get()->Press('D'))
	//	position.x += 100 * Time::Delta();
}

void Rect::GUI()
{
	//ImGui::Begin("Rect", &bOpen);
	//{
	//	ImGui::InputFloat3(MakeLable("Pos").c_str(), position, 2);
	//	ImGui::InputFloat3(MakeLable("Size").c_str(), size, 2);
	//	ImGui::SliderAngle(MakeLable("Rot").c_str(), &rotation);
	//	if (ImGui::ColorPicker4("Color", color))
	//		UpdateColor();
	//}
	//ImGui::End();
}

void Rect::UpdateWorld()
{
	//D3DXMatrixTranslation(&T, position.x, position.y, position.z);
	//D3DXMatrixScaling(&S, size.x, size.y, size.z);
	//D3DXMatrixRotationZ(&R, rotation);
	DirectX::XMMATRIX S, R, T, W;
	T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	S = DirectX::XMMatrixScaling(size.x, size.y, size.z);
	R = DirectX::XMMatrixRotationZ((float)D3DXToRadian(rotation));

	W = S * R * T;
	XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(W));
	//wb->SetWorld(world);

	//D3DXVec3TransformCoord(&edge->LT, &vertices[1].position, &world);
	//D3DXVec3TransformCoord(&edge->RB, &vertices[2].position, &world);

	DirectX::XMStoreFloat3(&edge->LT, DirectX::XMVector3TransformCoord(XMLoadFloat3(&vertices[3].position), W));
	DirectX::XMStoreFloat3(&edge->RB, DirectX::XMVector3TransformCoord(XMLoadFloat3(&vertices[2].position), W));
}

void Rect::UpdateColor()
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	mDeviceContext->Map(mVertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		for (VertexColor& v : vertices)
			v.color = color;

		memcpy(subResource.pData, vertices.data(), vertices.size() * (UINT)mVertexBuffer->GetStride());
	}
	mDeviceContext->Unmap(mVertexBuffer->GetResource(), 0);
}

std::string Rect::MakeLable(std::string preFix)
{
	std::string label = preFix + "##" + std::to_string((UINT)this);
	return label;
}

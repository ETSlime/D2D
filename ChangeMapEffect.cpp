#pragma once

#include "ChangeMapEffect.h"
#include "Timer.h"

ChangeMapEffect::ChangeMapEffect()
{
	SetVertices();
	SetIndices();
	CreateRenderResource(vertices, indices, ShaderPath + L"FadeShader.hlsl");

	timeBuffer = new UploadBuffer<float>(mDevice, mDeviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);
}

ChangeMapEffect::~ChangeMapEffect()
{
	SAFE_DELETE(timeBuffer);
}

void ChangeMapEffect::SetVertices()
{
	vertices.assign(4, SimpleVertex());

	vertices[0].position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);


	vertices[1].position = DirectX::XMFLOAT3(1000.0f, 1000.0f, 0.0f);

	vertices[2].position = DirectX::XMFLOAT3(1000.0f, 0.0f, 0.0f);

	vertices[3].position = DirectX::XMFLOAT3(0.0f, 1000.0f, 0.0f);
}

void ChangeMapEffect::SetIndices()
{
	indices = { 0, 1, 2, 0, 3, 1 };
}

void ChangeMapEffect::Update()
{

	if (isFadingIn)
	{
		float deltaTime = Timer::Delta();
		timeParameter -= deltaTime * fadeSpeed;
		if (timeParameter <= 0.0f)
		{
			timeParameter = 0.0f;
			isFadingIn = false;
			SetFading(false);
		}
		timeBuffer->MapData(mDeviceContext, timeParameter);
	}
	else if (isFadingOut)
	{
		float deltaTime = Timer::Delta();
		timeParameter += deltaTime * fadeSpeed;
		if (timeParameter >= 1.0f)
		{
			timeParameter = 1.0f;
			isFadingOut = false;
			
		}
		timeBuffer->MapData(mDeviceContext, timeParameter);
	}
}

void ChangeMapEffect::Render()
{
	__super::Render();

	mDeviceContext->PSSetConstantBuffers(0, 1, timeBuffer->Resource());

	mDeviceContext->DrawIndexed(indexCount, 0, 0);
}

void ChangeMapEffect::StartFadeOut()
{
	isFadingOut = true;
	timeParameter = 0.0f;
}

void ChangeMapEffect::StartFadeIn()
{
	isFadingIn = true;
	timeParameter = 1.0f;
}

//void ChangeMapEffect::Initialize()
//{
//    // 加载和编译着色器
//    ID3DBlob* psBlob = nullptr;
//    D3DCompileFromFile(L"_Shaders/FadeShader.hlsl", nullptr, nullptr, "PS", "ps_4_0", 0, 0, &psBlob, nullptr);
//    device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &fadeShader);
//    psBlob->Release();
//
//    // 创建时间缓冲区
//
//    timeBuffer = new UploadBuffer<float>(device, context, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);
//}
//
//void ChangeMapEffect::Update(float deltaTime)
//{
//    if (isFadingOut)
//    {
//        timeParameter += deltaTime * fadeSpeed;
//        if (timeParameter >= 1.0f)
//        {
//            timeParameter = 1.0f;
//            isFadingOut = false;
//            // 切换地图
//            // Render new map here
//            StartFadeIn();
//        }
//    }
//    else if (isFadingIn)
//    {
//        timeParameter -= deltaTime * fadeSpeed;
//        if (timeParameter <= 0.0f)
//        {
//            timeParameter = 0.0f;
//            isFadingIn = false;
//        }
//    }
//
//    // update time buffer
//    timeBuffer->MapData(context, timeParameter);
//    //D3D11_MAPPED_SUBRESOURCE mappedResource;
//    //context->Map(timeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//    //*(float*)mappedResource.pData = timeParameter;
//    //context->Unmap(timeBuffer, 0);
//}
//
//void ChangeMapEffect::Render()
//{
//    context->PSSetShader(fadeShader, nullptr, 0);
//    context->PSSetConstantBuffers(0, 1, timeBuffer->Resource());
//    // draw full screen square
//    context->Draw(4, 0);
//}
//


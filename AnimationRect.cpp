#include "AnimationRect.h"
#include "States.h"

AnimationRect::AnimationRect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size)
	:TextureRect(position, size, 0.0f)
{
	SetShader(ShaderPath + L"Animation.hlsl");


	{
		D3D11_SAMPLER_DESC desc;
		States::GetSamplerDesc(&desc);
		States::CreateSampler(&desc, &point[0]);

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		States::CreateSampler(&desc, &point[1]);
	}

	{
		D3D11_BLEND_DESC desc;
		States::GetBlendDesc(&desc);
		States::CreateBlend(&desc, &bPoint[0]);

		desc.RenderTarget[0].BlendEnable = true;
		States::CreateBlend(&desc, &bPoint[1]);
	}

	control = new PlayerControl();

}

AnimationRect::~AnimationRect()
{
}

void AnimationRect::Update()
{
	control->SetAnimator(&animator);

	MapVertexBuffer();
	{
		vertices[0].uv.x = animator->GetCurrentFrame().x;
		vertices[0].uv.y = animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y;

		vertices[1].uv.x = animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x;
		vertices[1].uv.y = animator->GetCurrentFrame().y;

		vertices[2].uv.x = animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x;
		vertices[2].uv.y = animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y;

		vertices[3].uv.x = animator->GetCurrentFrame().x;
		vertices[3].uv.y = animator->GetCurrentFrame().y;

	}
	UnmapVertexBuffer();

	__super::Update();

}

void AnimationRect::Render()
{
	srv = animator->GetCurrentSRV();


	mDeviceContext->PSSetSamplers(0, 1, &point[1]);
	mDeviceContext->OMSetBlendState(bPoint[1], nullptr, (UINT)0xFFFFFFFFFF);
	__super::Render();
	mDeviceContext->PSSetSamplers(0, 1, &point[0]);
	mDeviceContext->OMSetBlendState(bPoint[0], nullptr, (UINT)0xFFFFFFFFFF);
}

void AnimationRect::Move()
{
	//control->Move(VK_LEFT, position, L"WalkL");
	//control->Move(VK_RIGHT, position, L"WalkR");
	//control->Move(VK_UP, position, L"WalkU");
	//control->Move(VK_DOWN, position, L"WalkD");

	control->UpdatePosition(position);
}

bool AnimationRect::AABB(BoundingBox* other)
{
	if (box->AABB(other) == true)
	{
		isCollid = true;
		return true;
	}
	else
	{

		return false;
	}

}

void AnimationRect::SetclipName(std::wstring clipname1, std::wstring clipname2)
{
	this->clipNameL = clipname1;
	this->clipNameR = clipname2;
}

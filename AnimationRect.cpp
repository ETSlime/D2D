#include "AnimationRect.h"
#include "States.h"
#include "Player.h"

AnimationRect::AnimationRect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, EventType type)
	:TextureRect(position, size, 0.0f, false)
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

	if (type == EventType::PLAYER)
		control = new PlayerControl();

}

AnimationRect::~AnimationRect()
{
}

void AnimationRect::Update()
{
	if (control)
	{
		control->SetAnimator(&animator);
		control->SetPlayer(static_cast<Player*>(currentEvent));
	}


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

	UpdateBoundingBox();

	__super::Update();

}

void AnimationRect::Render()
{
	srv = animator->GetCurrentSRV();


	//mDeviceContext->PSSetSamplers(0, 1, &point[1]);
	//mDeviceContext->OMSetBlendState(bPoint[1], nullptr, (UINT)0xFFFFFFFFFF);
	__super::Render();
	//mDeviceContext->PSSetSamplers(0, 1, &point[0]);
	//mDeviceContext->OMSetBlendState(bPoint[0], nullptr, (UINT)0xFFFFFFFFFF);
}

void AnimationRect::Move()
{
	if (control)
	{
		Map::get_instance().UpdateCollisionBoxes();
		control->UpdatePlayerPosition(position);
	}
		
}

bool AnimationRect::AABB(BoundingBox* other)
{
	if (boundingBox->AABB(other) == true)
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

void AnimationRect::UpdateBoundingBox()
{
	if (boundingBox)
	{
		boundingBox->SetEdge(
			DirectX::XMFLOAT3(position.x, position.y + TileHeight, 0.0f),
			DirectX::XMFLOAT3(position.x + TileWidth, position.y, 0.0f));
	}
	else
	{
		boundingBox = new BoundingBox(new RectEdge(
			DirectX::XMFLOAT3(position.x, position.y + TileHeight, 0.0f),
			DirectX::XMFLOAT3(position.x + TileWidth, position.y, 0.0f)));
	}

}

void AnimationRect::SetBoundingBoxType(ColliderType type)
{
	boundingBox->colliderType = type;
}

void AnimationRect::SetOnCollision(CollisionCallback onCollision)
{
	boundingBox->onCollision = onCollision;
}
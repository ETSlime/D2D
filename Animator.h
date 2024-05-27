#pragma once

#include "Util.h"
#include "Texture2D.h"
#include "DirectHelper.h"

class AnimationClip
{
public:
	friend class Animator;

	AnimationClip(std::wstring clipName, Texture2D* srcTex, UINT frameCount, DirectX::XMFLOAT2 startPos, DirectX::XMFLOAT2 endPos, float playRate, bool isVertical = false, bool bReversed = false);

private:
	std::wstring clipName = L"";
	std::vector<DirectX::XMFLOAT2> keyFrames;
	UINT frameCount = 0;
	ID3D11ShaderResourceView* srv = nullptr;
	DirectX::XMFLOAT2 texelFrameSize = Values::ZeroVec2;
	float playRate = 0.0f;
	bool bReversed = false;
	bool isVertical = false;
};

class Animator
{
public:
	Animator();
	~Animator();

	void Update();
	DirectX::XMFLOAT2 GetCurrentFrame() { return currentFrame; }
	DirectX::XMFLOAT2 GetTexelFrameSize() { return currentClip->texelFrameSize; }
	ID3D11ShaderResourceView* GetCurrentSRV() { return currentClip->srv; }

	void AddAnimClip(AnimationClip* animClip);
	void SetCurrentAnimClip(std::wstring clipName);
	AnimationClip* GetCurrentClip() { return currentClip; }

	void SetAnim(AnimationClip* clip);

private:
	std::unordered_map<std::wstring, AnimationClip*> animClips;
	bool CheckExist(std::wstring clipName) { return animClips.find(clipName) != animClips.end(); }

	AnimationClip* currentClip = nullptr;
	UINT currentFrameIndex = 0;
	DirectX::XMFLOAT2 currentFrame = Values::ZeroVec2;

	float deltaTime = 0.0f;

	float Rate = 0.0f;
};
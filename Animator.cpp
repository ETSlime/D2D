#include "Animator.h"
#include "Timer.h"

AnimationClip::AnimationClip(std::wstring clipName, Texture2D* srcTex, UINT frameCount, DirectX::XMFLOAT2 startPos, DirectX::XMFLOAT2 endPos, float playRate, bool bReversed)
	:clipName(clipName), frameCount(frameCount), playRate(playRate), bReversed(bReversed)
{
	srv = srcTex->GetSRV();  //ﾀﾌｹﾌﾁ・ｼﾒｽｺｸｦ ｰ｡ﾁｮｿﾍｼｭ ｴ・ﾔ

	float imageWidth = srcTex->GetWidth();		//ﾇ・ｻ鄙・ﾏｴﾂ ﾀﾌｹﾌﾁﾇ ｰ｡ｷﾎﾅｩｱ・ｱｸﾇﾏｱ・
	float imageHeight = srcTex->GetHeight();	//ﾇ・ｻ鄙・ﾏｴﾂ ﾀﾌｹﾌﾁﾇ ｼｼｷﾎﾅｩｱ・ｱｸﾇﾏｱ・

	DirectX::XMFLOAT2 clipSize = DirectX::XMFLOAT2(endPos.x - startPos.x, endPos.y - startPos.y);		//ﾇ・ｻ鄙・ﾒ ﾅｬｸｳﾀﾇ ｻ鄲ﾌﾁ・ｱｸﾇﾏｱ・
	DirectX::XMFLOAT2 frameSize;
	frameSize.x = clipSize.x / frameCount;		//ﾇﾑﾇﾁｷｹﾀﾓﾀﾇ ｻ鄲ﾌﾁ鋕ｦ ｱｸﾇﾏｱ・
	frameSize.y = clipSize.y;

	texelFrameSize.x = frameSize.x / imageWidth;	//ﾇﾏｳｪﾀﾇ ﾅﾘｼｿ ﾅｩｱ篋ｦ ｱｸﾇﾏｱ・
	texelFrameSize.y = frameSize.y / imageHeight;

	DirectX::XMFLOAT2 texelStartPos;
	texelStartPos.x = startPos.x / imageWidth;		//ﾇﾏｳｪﾀﾇ ﾅﾘｼｿﾀﾇ ｽﾃﾀﾛﾁ｡ﾀｻ ｱｸﾇﾏｱ・
	texelStartPos.y = startPos.y / imageHeight;

	DirectX::XMFLOAT2 keyframe;

	for (UINT i = 0; i < frameCount; i++)
	{
		keyframe.x = texelStartPos.x + (texelFrameSize.x * i);
		keyframe.y = texelStartPos.y;
		keyFrames.push_back(keyframe);
	}
}

Animator::Animator()
{
}

Animator::~Animator()
{
	for (auto clip : animClips)
		SAFE_DELETE(clip.second);
}

void Animator::Update()
{
	if (deltaTime > currentClip->playRate)
	{
		if (currentClip->bReversed == false)
		{
			currentFrameIndex++;

			if (currentFrameIndex == currentClip->frameCount)
			{
				Rate = Timer::Delta();
				currentFrameIndex = 0;
			}

			currentFrame = currentClip->keyFrames[currentFrameIndex];
		}
		else
		{
			currentFrameIndex--;

			if (currentFrameIndex == -1)
				currentFrameIndex = currentClip->frameCount - 1;

			currentFrame = currentClip->keyFrames[currentFrameIndex];
		}
		deltaTime = 0.0f;
	}
	else
	{
		deltaTime += Timer::Delta();
	}
}

void Animator::AddAnimClip(AnimationClip* animClip)
{
	animClips.insert(make_pair(animClip->clipName, animClip));
}

void Animator::SetCurrentAnimClip(std::wstring clipName)
{
	if (currentClip == nullptr && CheckExist(clipName) == true)
		currentClip = animClips.find(clipName)->second;
	else if (currentClip != nullptr && currentClip->clipName != clipName)
		currentClip = animClips.find(clipName)->second;
	else if (currentClip->clipName == clipName)
		return;

	if (CheckExist(clipName))
	{
		currentClip = animClips.find(clipName)->second;
		deltaTime = 0.0f;

		if (currentClip->bReversed == true)
			currentFrameIndex = currentClip->frameCount - 1;
		else
			currentFrameIndex = 0;

		currentFrame = currentClip->keyFrames[currentFrameIndex];
	}
}

void Animator::SetAnim(AnimationClip* clip)
{


	AddAnimClip(clip);


}

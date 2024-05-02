#pragma once
#include "Texture2D.h"
#include "Util.h"
#include "DirectHelper.h"
#include <unordered_map>

class UIClip
{
public:
	friend class UIalphabet;

	UIClip(std::wstring clipName, Texture2D* srcTex, DirectX::XMFLOAT2 startPos, DirectX::XMFLOAT2 endPos);

private:
	std::wstring clipName = L"";
	ID3D11ShaderResourceView* srv = nullptr;
	DirectX::XMFLOAT2 texelFrameSize = Values::ZeroVec2;
	DirectX::XMFLOAT2 texelStartPos;
};

class UIalphabet
{
public:
	UIalphabet();
	~UIalphabet();

	void Update();
	DirectX::XMFLOAT2 GetCurrentFrame() { return currentFrame; }
	DirectX::XMFLOAT2 GetTexelFrameSize() { return currentClip->texelFrameSize; }
	ID3D11ShaderResourceView* GetCurrentSRV() { return currentClip->srv; }

	void AddUIClip(UIClip* UIClip);
	void SetCurrentUIClip(std::wstring clipName);
	UIClip* GetCurrentClip() { return currentClip; }

	void SetUI(UIClip* clip);

private:
	std::unordered_map<std::wstring, UIClip*> UIclips;
	bool CheckExist(std::wstring clipName) { return UIclips.find(clipName) != UIclips.end(); }

	UIClip* currentClip = nullptr;
	UINT currentFrameIndex = 0;
	DirectX::XMFLOAT2 currentFrame = Values::ZeroVec2;


};
#include "Util.h"
#include "UIalphabet.h"
#include "Texture2D.h"

UIClip::UIClip(std::wstring clipName, Texture2D* srcTex, DirectX::XMFLOAT2 startPos, DirectX::XMFLOAT2 endPos)
	:clipName(clipName)
{
	srv = srcTex->GetSRV();  //�̹���E�ҽ��� �����ͼ� ��E�

	float imageWidth = srcTex->GetWidth();		//����E�翁Eϴ� �̹����� ����ũ��E���ϱ�E
	float imageHeight = srcTex->GetHeight();	//����E�翁Eϴ� �̹����� ����ũ��E���ϱ�E
	DirectX::XMFLOAT2 clipSize = DirectX::XMFLOAT2(endPos.x - startPos.x, endPos.y - startPos.y);
	DirectX::XMFLOAT2 frameSize;
	frameSize.x = clipSize.x;		//���������� ������Ԧ ���ϱ�E
	frameSize.y = clipSize.y;

	texelFrameSize.x = frameSize.x / imageWidth;	//�ϳ��� �ؼ� ũ�⸦ ���ϱ�E
	texelFrameSize.y = frameSize.y / imageHeight;


	texelStartPos.x = startPos.x / imageWidth;		//�ϳ��� �ؼ��� �������� ���ϱ�E
	texelStartPos.y = startPos.y / imageHeight;


}

UIalphabet::UIalphabet()
{
}

UIalphabet::~UIalphabet()
{
	for (auto clip : UIclips)
		SAFE_DELETE(clip.second);
}
void UIalphabet::Update()
{

}

void UIalphabet::AddUIClip(UIClip* UIClip)
{
	UIclips.insert(make_pair(UIClip->clipName, UIClip));
}

void UIalphabet::SetCurrentUIClip(std::wstring clipName)
{
	if (currentClip == nullptr && CheckExist(clipName) == true)
		currentClip = UIclips.find(clipName)->second;
	else if (currentClip != nullptr && currentClip->clipName != clipName)
		currentClip = UIclips.find(clipName)->second;
	else if (currentClip->clipName == clipName)
		return;


	if (CheckExist(clipName))
	{
		currentClip = UIclips.find(clipName)->second;
		currentFrame = currentClip->texelStartPos;
	}
}

void UIalphabet::SetUI(UIClip* clip)
{


	AddUIClip(clip);


}

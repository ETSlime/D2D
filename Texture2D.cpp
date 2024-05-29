#include "Path.h"
#include "Texture2D.h"

std::vector<TextureDesc> Textures::descs;

Texture2D::Texture2D(std::wstring filePath, bool repeat):filePath(filePath), readRepeatedTex(repeat)
{
	Textures::Load(mDevice, this, readRepeatedTex);
}

void Texture2D::ReadPixel(std::vector<DirectX::XMFLOAT4>* pixels)
{
	ID3D11Texture2D* texture;
	srv->GetResource((ID3D11Resource**)&texture);

	ReadPixel(mDevice,mDeviceContext, texture, pixels);
}

void Texture2D::ReadPixel(ID3D11Device* device, ID3D11DeviceContext* deviceContext, 
	ID3D11Texture2D* texture, std::vector<DirectX::XMFLOAT4>* pixels)
{
	//D3D11_TEXTURE2D_DESC srcDesc;
	//texture->GetDesc(&srcDesc);

	//D3D11_TEXTURE2D_DESC destDesc;
	//ZERO(destDesc);

	//destDesc.Usage = D3D11_USAGE_STAGING;
	//destDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	//destDesc.Width = srcDesc.Width;
	//destDesc.Height = srcDesc.Height;
	//destDesc.MipLevels = 1;
	//destDesc.ArraySize = 1;
	//destDesc.Format = srcDesc.Format;
	//destDesc.SampleDesc = srcDesc.SampleDesc;

	//ID3D11Texture2D* destTex = nullptr;

	//HRESULT hr = device->CreateTexture2D(&destDesc, nullptr, &destTex);
	//Assert(SUCCEEDED(hr));

	////D3DX11LoadTextureFromTexture(deviceContext, texture, nullptr, destTex);

	//UINT* colors = new UINT[destDesc.Width * destDesc.Height];

	//D3D11_MAPPED_SUBRESOURCE subResource;
	//deviceContext->Map(destTex, 0, D3D11_MAP_READ, 0, &subResource);
	//{
	//	memcpy(colors, subResource.pData, sizeof(UINT) * destDesc.Width * destDesc.Height);
	//}
	//deviceContext->Unmap(destTex, 0);

	//int pixelNum = destDesc.Width * destDesc.Height;

	//UINT* colors1 = new UINT[destDesc.Width * destDesc.Height];
	//UINT* colors2 = new UINT[destDesc.Width * destDesc.Height];
	//UINT* colors3 = new UINT[destDesc.Width * destDesc.Height];
	//UINT* colors4 = new UINT[destDesc.Width * destDesc.Height];

	//for (int i = 0; i < pixelNum; i++)
	//{
	//	colors1[i] = (colors[i] & 0xFF000000);
	//	colors2[i] = (colors[i] & 0x00FF0000);
	//	colors3[i] = (colors[i] & 0x0000FF00);
	//	colors4[i] = (colors[i] & 0x000000FF);

	//	colors2[i] = colors[i] >> 16;
	//	colors4[i] = colors[i] << 16;

	//	colors[i] = colors1[i] | colors2[i] | colors3[i] | colors4[i];
	//}

	//pixels->assign(&colors[0], &colors[destDesc.Width * destDesc.Height - 1]);

	//SAFE_DELETE_ARRAY(colors);
	//SafeRelease(&destTex);


}

void Textures::Delete()
{
	for (TextureDesc desc : descs)
		SafeRelease(&desc.srv);
}

void Textures::Load(ID3D11Device* device, Texture2D* texture, bool repeat)
{
	HRESULT hr;
	DirectX::TexMetadata metaData;

	std::wstring ext = Path::GetExtension(texture->filePath);
	if (ext == L"tga")
	{
		hr = GetMetadataFromTGAFile(texture->filePath.c_str(), metaData);
		Assert(SUCCEEDED(hr));
	}
	else if (ext == L"dds")
	{
		hr = GetMetadataFromDDSFile(texture->filePath.c_str(), DirectX::DDS_FLAGS_NONE, metaData);
		Assert(SUCCEEDED(hr));
	}
	else
	{
		hr = GetMetadataFromWICFile(texture->filePath.c_str(), DirectX::WIC_FLAGS_NONE, metaData);
		Assert(SUCCEEDED(hr));
	}

	UINT width = static_cast<UINT>(metaData.width);
	UINT height = static_cast<UINT>(metaData.height);

	TextureDesc desc;
	desc.filePath = texture->filePath;
	desc.width = width;
	desc.height = height;

	TextureDesc exist;
	bool bExist = false;
	for (TextureDesc temp : descs)
	{
		if (desc == temp)
		{
			bExist = true;
			exist = temp;

			break;
		}
	}

	if (bExist == true && !repeat)
	{
		texture->srv = exist.srv;
	}
	else
	{
		DirectX::ScratchImage image;

		if (ext == L"tga")
		{
			hr = DirectX::LoadFromTGAFile(texture->filePath.c_str(), &metaData, image);
			Assert(SUCCEEDED(hr));
		}
		else if (ext == L"dds")
		{
			hr = DirectX::LoadFromDDSFile(texture->filePath.c_str(), DirectX::DDS_FLAGS_NONE, &metaData, image);
			Assert(SUCCEEDED(hr));
		}
		else
		{
			hr = DirectX::LoadFromWICFile(texture->filePath.c_str(), DirectX::WIC_FLAGS_NONE, &metaData, image);
			Assert(SUCCEEDED(hr));
		}

		ID3D11ShaderResourceView* srv = nullptr;
		hr = CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), metaData, &srv);
		Assert(SUCCEEDED(hr));

		desc.srv = srv;
		texture->srv = srv;
		texture->metaData = metaData;

		descs.push_back(desc);
	}

}

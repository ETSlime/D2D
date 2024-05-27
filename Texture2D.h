#pragma once

#include "Util.h"
#include "MagicTowerApp.h"

struct TextureDesc
{
	std::wstring filePath = L"";
	UINT width = 0;
	UINT height = 0;
	ID3D11ShaderResourceView* srv = nullptr;

	bool operator==(const TextureDesc& other) const
	{
		bool b = true;

		b &= filePath == other.filePath;
		b &= width == other.width;
		b &= height == other.height;

		return b;
	}
};

class Texture2D
{
public:
	Texture2D(std::wstring filePath);

	void ReadPixel(std::vector<DirectX::XMFLOAT4>* pixels);
	static void ReadPixel(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
		ID3D11Texture2D* texture, std::vector<DirectX::XMFLOAT4>* pixels);

	ID3D11ShaderResourceView* GetSRV() { return srv; }
	float GetWidth() { return (float)metaData.width; }
	float GetHeight() { return (float)metaData.height; }
	std::wstring GetFilePath() { return filePath; }

private:
	friend class Textures;

	std::wstring filePath = L"";
	ID3D11ShaderResourceView* srv = nullptr;
	DirectX::TexMetadata metaData;
	ID3D11Device* mDevice = MagicTowerApp::get_instance().mDevice;
	ID3D11DeviceContext* mDeviceContext = MagicTowerApp::get_instance().mDeviceContext;
};

class Textures
{
public:
	static void Delete();

private:
	friend class Texture2D;

	static void Load(ID3D11Device* device, Texture2D* texture);
	static std::vector<TextureDesc> descs;
};

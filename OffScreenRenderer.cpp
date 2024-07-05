#include "OffScreenRenderer.h"
#include "MagicTowerApp.h"

OffScreenRenderer::OffScreenRenderer(ID2D1Factory* d2dFactory, D2DResource* D2DResource, 
    OffScreenRenderMode mode, EffectParameters params, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation)
    :RectBase(position, size, rotation), pD2DFactory(d2dFactory), mD2DResource(D2DResource), renderMode(mode),
    effectParams(params), offscreenTexture(nullptr), offscreenRTV(nullptr), offscreenSRV(nullptr), sampleState(nullptr)
{
    InitializeResources();
}

OffScreenRenderer::~OffScreenRenderer()
{
    CleanupResources();
}

void OffScreenRenderer::InitializeResources()
{

    // Create blend state
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    mDevice->CreateBlendState(&blendDesc, &blendState);

    // Create offscreen texture
    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = size.x * 2;
    textureDesc.Height = size.y * 2;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    mDevice->CreateTexture2D(&textureDesc, nullptr, &offscreenTexture);

    // Create render target view
    mDevice->CreateRenderTargetView(offscreenTexture, nullptr, &offscreenRTV);

    // Create shader resource view
    mDevice->CreateShaderResourceView(offscreenTexture, nullptr, &offscreenSRV);


    offscreenTexture->QueryInterface(__uuidof(IDXGISurface), reinterpret_cast<void**>(&pOffscreenTextureSurface));


    // Create sampler state
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    mDevice->CreateSamplerState(&sampDesc, &sampleState);

    // Create screen quad vertex buffer
    SetVertices();
    SetIndices();
    CreateRenderResource(vertices, indices, L"");

    SetEffectMode(renderMode);
}

void OffScreenRenderer::SetVertices()
{
    vertices.assign(4, VertexTexture());

    vertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
    vertices[0].uv = DirectX::XMFLOAT2(0.0f, 1.0f);

    vertices[1].position = DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f);
    vertices[1].uv = DirectX::XMFLOAT2(0.0f, 0.0f);

    vertices[2].position = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
    vertices[2].uv = DirectX::XMFLOAT2(1.0f, 0.0f);

    vertices[3].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);
    vertices[3].uv = DirectX::XMFLOAT2(1.0f, 1.0f);
}

void OffScreenRenderer::SetIndices()
{
    indices = { 0, 1, 2, 0, 2, 3 };
}

void OffScreenRenderer::CleanupResources()
{
    SafeRelease(&offscreenTexture);
    SafeRelease(&offscreenRTV);
    SafeRelease(&offscreenSRV);
    SafeRelease(&sampleState);
    SafeRelease(&blendState);

    SAFE_DELETE(fadeBuffer);
    SAFE_DELETE(shakeMatrixBuffer);
    
}

void OffScreenRenderer::Begin()
{
    mDeviceContext->OMSetRenderTargets(1, &offscreenRTV, nullptr);

    // Clear the render target
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    mDeviceContext->ClearRenderTargetView(offscreenRTV, clearColor);

    // Enable blend state
    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    mDeviceContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);

    // Switch pD2DRenderTarget to offscreen texture surface
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
    // Release the existing Direct2D render target if it exists
    if (mD2DResource->pD2DRenderTarget)
    {
        SafeRelease(&mD2DResource->pD2DRenderTarget);
    }
    pD2DFactory->CreateDxgiSurfaceRenderTarget(pOffscreenTextureSurface, &props, &(mD2DResource->pD2DRenderTarget));
}

void OffScreenRenderer::Update()
{
    UpdateWorld();

    if (enableShakeEffect)
    {
        effectTimer += Timer::Delta();
        if (effectTimer > shakeDuration)
        {
            enableShakeEffect = false;
            MagicTowerApp::get_instance().SetShakeEffect(enableShakeEffect);
            effectTimer = 0.0f;
        }
    }
    else if (enableBlurEffect)
    {
        effectTimer += Timer::Delta();
        if (effectTimer > blurDuration)
        {
            enableBlurEffect = false;
            MagicTowerApp::get_instance().SetShakeEffect(enableBlurEffect);
            effectTimer = 0.0f;
        }
    }
    else if (enableFadeEffect)
    {
        effectTimer += Timer::Delta();
        if (effectTimer > fadeDuration)
        {
            enableFadeEffect = false;
            MagicTowerApp::get_instance().SetShakeEffect(enableFadeEffect);
            effectTimer = 0.0f;
        }
    }
}

void OffScreenRenderer::Render()
{
    // Disable blend state
    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    mDeviceContext->OMSetBlendState(nullptr, blendFactor, 0xffffffff);

    // Render the offscreen texture to the screen quad
    UINT offset = 0;
    mDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer->GetBuffer(), mVertexBuffer->GetStride(), &offset);
    mDeviceContext->IASetIndexBuffer(mIndexBuffer->GetResource(), DXGI_FORMAT_R32_UINT, 0);
    mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    mVertexShader->SetShader();
    mPixelShader->SetShader();

    mDeviceContext->VSSetConstantBuffers(0, 1, mWorldBuffer->Resource());
    mDeviceContext->PSSetShaderResources(0, 1, &offscreenSRV);
    mDeviceContext->PSSetSamplers(0, 1, &sampleState);


    mDeviceContext->DrawIndexed(indexCount, 0, 0);
}

void OffScreenRenderer::ApplyEffect(const EffectParameters& params)
{
    switch (renderMode)
    {
    case OffScreenRenderMode::SHAKE:
    {
        // Generate shake matrix
        float offsetX = ((rand() % 100) / 5000.0f) - 0.01f * params.shakeIntensity;
        float offsetY = ((rand() % 100) / 5000.0f) - 0.01f * params.shakeIntensity;

        DirectX::XMMATRIX shakeMatrix = DirectX::XMMatrixTranslation(offsetX, offsetY, 0.0f);

        DirectX::XMFLOAT4X4 shake;
        XMStoreFloat4x4(&shake, DirectX::XMMatrixTranspose(shakeMatrix));
        shakeMatrixBuffer->MapData(mDeviceContext, shake);

        mDeviceContext->VSSetConstantBuffers(2, 1, shakeMatrixBuffer->Resource());

        // Render the offscreen texture to the screen quad with the shake effect
        Render();
        break;
    }

    case OffScreenRenderMode::BLUR:
        Render();
        break;
    case OffScreenRenderMode::FADE:
        fadeBuffer->MapData(mDeviceContext, params.fadeAlpha);
        mDeviceContext->PSSetConstantBuffers(0, 1, fadeBuffer->Resource());
        Render();

        break;
    case OffScreenRenderMode::NONE:
    default:
        // No effect applied
        break;
    }
}

void OffScreenRenderer::SetEffectMode(OffScreenRenderMode mode)
{
    renderMode = mode;
    switch (renderMode)
    {
    case OffScreenRenderMode::SHAKE:
        enableShakeEffect = true;
        SetShader(ShaderPath + L"ShakeEffect.hlsl");
        mInputLayout->Create(VertexTexture::descs, VertexTexture::count, this->mVertexShader->GetBlob());
        shakeMatrixBuffer = new UploadBuffer<DirectX::XMFLOAT4X4>(mDevice, mDeviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);
        break;
    case OffScreenRenderMode::BLUR:
        SetShader(ShaderPath + L"BlurEffect.hlsl");
        mInputLayout->Create(VertexTexture::descs, VertexTexture::count, this->mVertexShader->GetBlob());
        break;
    case OffScreenRenderMode::FADE:
        fadeBuffer = new UploadBuffer<float>(mDevice, mDeviceContext, 1, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC);
        SetShader(ShaderPath + L"FadeEffect.hlsl");
        mInputLayout->Create(VertexTexture::descs, VertexTexture::count, this->mVertexShader->GetBlob());
        break;
    case OffScreenRenderMode::NONE:
    default:
        break;
    }

}
#pragma once

#include "Util.h"
#include "RectBase.h"

struct EffectParameters
{
    float shakeIntensity;
    float blurAmount;
    float fadeAlpha;
};

enum class OffScreenRenderMode
{
    SHAKE,
    BLUR,
    FADE,
    NONE
};
class OffScreenRenderer:public RectBase<VertexTexture>
{
public:

    OffScreenRenderer(ID2D1Factory* d2dFactory, D2DResource* D2DResource, OffScreenRenderMode mode, 
        EffectParameters params, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 size, float rotation);
    ~OffScreenRenderer();

    void Begin();
    void SetVertices() override;
    void SetIndices() override;
    void Render() override;
    void Update() override;


    void ApplyEffect(const EffectParameters& params);

    void SetShakeDuration(float duration) { shakeDuration = duration; }

    void SetEffectMode(OffScreenRenderMode mode);
    void SetEffectParameters(EffectParameters params) { effectParams = params; }
    OffScreenRenderMode GetEffectMode() { return renderMode; }
    EffectParameters GetEffectParams() { return effectParams; }
private:
    void InitializeResources();
    void CleanupResources();

    std::vector<VertexTexture> vertices;
    std::vector<UINT> indices;

    ID3D11Texture2D* offscreenTexture;
    ID3D11RenderTargetView* offscreenRTV;
    ID3D11ShaderResourceView* offscreenSRV;
    ID3D11SamplerState* sampleState;
    ID3D11BlendState* blendState;

    D2DResource* mD2DResource = nullptr;
    ID2D1Factory* pD2DFactory = nullptr;
    IDXGISurface* pOffscreenTextureSurface = nullptr;

    UploadBuffer<float>* fadeBuffer = nullptr;
    UploadBuffer<DirectX::XMFLOAT4X4>* shakeMatrixBuffer = nullptr;

    bool enableShakeEffect = false;
    bool enableFadeEffect = false;
    bool enableBlurEffect = false;
    float effectTimer = 0.0f;
    float shakeDuration = 1.0f;
    float fadeDuration = 1.0f;
    float blurDuration = 1.0f;

    OffScreenRenderMode renderMode;
    EffectParameters effectParams;
};


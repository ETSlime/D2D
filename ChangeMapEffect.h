#pragma once

#include "Util.h"
#include "RectBase.h"

class ChangeMapEffect:public RectBase<SimpleVertex>
{
public:

    ChangeMapEffect();
    ~ChangeMapEffect();
    
    void SetVertices() override;
    void SetIndices() override;
    void Update() override;
    void Render() override;
    void StartFadeOut();
    void StartFadeIn();

    void SetFading(bool fading) { isFading = fading; }
    bool GetFading() { return isFading; }
    float GetFadeSpeed() { return fadeSpeed; }

private:
    UploadBuffer<float>* timeBuffer;
    float timeParameter = 0.0f;
    bool isFadingOut = false;
    bool isFadingIn = false;
    bool isFading = false;
    float fadeSpeed = 1.0f; 

    std::vector<SimpleVertex> vertices;
    std::vector<UINT> indices;
};
// 在游戏主循环中调用
//void GameLoop()
//{
//    TeleportEffect teleportEffect;
//    teleportEffect.Initialize(device, context);
//
//    auto previousTime = std::chrono::high_resolution_clock::now();
//
//    while (true)
//    {
//        auto currentTime = std::chrono::high_resolution_clock::now();
//        std::chrono::duration<float> deltaTime = currentTime - previousTime;
//        previousTime = currentTime;
//
//        float deltaTimeSeconds = deltaTime.count();
//
//        // 更新传送效果
//        teleportEffect.Update(deltaTimeSeconds);
//
//        // 渲染传送效果
//        teleportEffect.Render(context);
//
//        // 其他游戏更新和渲染逻辑
//    }
//}

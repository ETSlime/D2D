#pragma once


#include "SingletonBase.h"
#include "Util.h"
    
class Camera:public SingletonBase<Camera>
{
public:

    // needs to be friend in order to
    // access the private constructor/destructor
    friend class SingletonBase<Camera>;
      
    
    ~Camera();

    // Get View/Proj matrices.
    DirectX::XMMATRIX GetView()const;
    DirectX::XMMATRIX GetProj()const;

    DirectX::XMFLOAT4X4 GetView4x4f()const;
    DirectX::XMFLOAT4X4 GetProj4x4f()const;

    void UpdateViewMatrix();

    // Build a view matrix for a left-handed coordinate system 
    // using a camera position, an up direction, and a focal point.
    void LookAtLH(const DirectX::XMFLOAT3& pos,
                const DirectX::XMFLOAT3& target, 
                const DirectX::XMFLOAT3& up);


    // Build a custom orthogonal projection matrix for a left-handed coordinate system.
    void OffCenterLH(float ViewLeft,
                   float ViewRight,
                   float ViewBottom,
                   float ViewTop,
                   float NearZ,
                   float FarZ);

private:

    Camera();

    

    bool mViewDirty = false;

    // view port
    DirectX::XMFLOAT4X4 mView = MathHelper::Identity4x4();
    DirectX::XMFLOAT4X4 mProj = MathHelper::Identity4x4();
};
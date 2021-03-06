#pragma once

//////////////
// INCLUDES //
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <mutex>

///////////////////////
// MY CLASS INCLUDES //

/////////////////
// FORWARD DEC //

class D3DClass{

public:
    ~D3DClass();
    static D3DClass* getInstance();

    bool Initialize();
    void Shutdown();

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();

    DirectX::XMFLOAT4X4 GetProjectionMatrix();
    DirectX::XMFLOAT4X4 GetWorldMatrix();
    DirectX::XMFLOAT4X4 GetOrthoMatrix();

    void BeginScene(float red, float green, float blue, float alpha);
    void EndScene();

    void TurnZBufferOn();
    void TurnZBufferOff();

    void TurnOnAlphaBlending();
    void TurnOffAlphaBlending();

    void TurnOnBackFaceCulling();
    void TurnOffBackFaceCulling();

private:
    D3DClass();
    D3DClass(const D3DClass&) {};
    D3DClass& operator=(const D3DClass&) {};

    static D3DClass* m_pInstance;
    static std::mutex instanceMutex;

    bool m_vsync_enabled;

    DirectX::XMFLOAT4X4 m_projectionMatrix;
    DirectX::XMFLOAT4X4 m_worldMatrix;
    DirectX::XMFLOAT4X4 m_orthoMatrix;
    
    IDXGISwapChain* m_swapChain;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_deviceContext;
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11Texture2D* m_depthStencilBuffer;
    ID3D11DepthStencilState* m_depthStencilState;
    ID3D11DepthStencilView* m_depthStencilView;
    ID3D11RasterizerState* m_rasterState;
    ID3D11DepthStencilState* m_depthDisabledStencilState;
    ID3D11BlendState* m_alphaEnableBlendingState;
    ID3D11BlendState* m_alphaDisableBlendingState;
};
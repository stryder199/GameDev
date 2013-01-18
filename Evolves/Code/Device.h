//=================================
// include guard
#ifndef Device_H_
#define Device_H_

//=================================
// forward declared dependencies


//=================================
// included dependencies
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include <D3DX10math.h>

class Device
{

private:
	int cBufferSize;


public:
	D3D_DRIVER_TYPE			g_driverType;
	D3D_FEATURE_LEVEL		g_featureLevel;
	ID3D11Device			*g_pd3dDevice;
	ID3D11DeviceContext		*g_pImmediateContext;
	IDXGISwapChain			*g_pSwapChain;
	ID3D11RenderTargetView	*g_pRenderTargetView;
	ID3D11VertexShader		*g_pVertexShader;
	ID3D11PixelShader		*g_pPixelShader;
	ID3D11PixelShader		*g_pPixelShaderSolid;
	ID3D11Buffer			*g_pConstantBuffer;
	ID3D11InputLayout		*g_pVertexLayout;
	ID3D11Texture2D         *g_pDepthStencil;
	ID3D11DepthStencilView  *g_pDepthStencilView;
	XMMATRIX				g_View;
	XMMATRIX				g_Projection

	Device();
	HRESULT init( HWND g_hWnd );
	void CleanupDevice();
	HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );
	HRESULT compilePShader(LPCSTR entryPoint);
	HRESULT compileVShader(LPCSTR entryPoint);
};

#endif /* Device_H_ */
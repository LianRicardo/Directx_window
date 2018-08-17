#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <d3dx11.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <fstream>
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")

class Capp
{
public:
	virtual ~Capp();
	int Run();
	ID3DBlob* compileShaders(std::string file, ID3DBlob* i);
	virtual bool Init();
	//virtual void Update(float dt);
	virtual void Render();
	struct colors
	{
		float red[4] = { 1.0f,0.0f,0.0f,1.0f };
		float green[4] = { 0.0f,1.0f,0.0f,1.0f };
		float blue[4] = { 0.0f,0.0f,1.0f,1.0f };
		float ligthblue[4] = { 0.392156899f, 0.584313750f, 0.929411829f, 1.0f };
		float guren[4] = { 0.862745166f, 0.078431375f, 0.235294133f, 1.0f };
		float cyan[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
		float magenta[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
		float yellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
	};
	colors c;

protected:
	Capp(HINSTANCE instance);
	bool Init_Window();
	bool Init_Device();
	bool Init_Triangle();
	HWND Ventana;
	HINSTANCE appinstance;
	UINT width;
	UINT height;
	std::string titulo;
	DWORD wndS;
	WNDCLASSEX wcex;

	//directx
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	IDXGISwapChain* pSwapChain;
	ID3D11RenderTargetView* pRenderTarget;
	D3D_FEATURE_LEVEL FeatureLevel;
	D3D_DRIVER_TYPE DriverTypes;
	D3D11_VIEWPORT viewport;
	ID3D11Texture2D* pBackBufferTextura;
	UINT DFlags;
	DXGI_SWAP_CHAIN_DESC swapchaindesc;

	//buffers y shaders and stuff para el triangulo
	ID3D11InputLayout* pLayout;
	ID3D11VertexShader* pVShader;
	ID3D11PixelShader* pPShader;
	ID3D11Buffer* pVertexBuffer;
	D3D11_BUFFER_DESC vertexbufferdesc;
	D3D11_SUBRESOURCE_DATA InitData;
	ID3DBlob *Id_VShader;
	ID3DBlob *Id_PShader;
	struct Vertex4
	{
		float x;
		float y;
		float z;
		D3DXCOLOR color;
	};
};

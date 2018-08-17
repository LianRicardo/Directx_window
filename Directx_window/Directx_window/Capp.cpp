#include "Capp.h"
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

Capp::Capp(HINSTANCE instance)
{
	appinstance = instance;
	Ventana = NULL;
	height = 600;
	width = 800;
	titulo = "ventana de lian";
	wndS = WS_OVERLAPPEDWINDOW;

	pDevice = nullptr;
	pDeviceContext = nullptr;
	pSwapChain = nullptr;
	DriverTypes= D3D_DRIVER_TYPE_HARDWARE;
	pBackBufferTextura = nullptr;
	FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	DFlags = 0;

	pLayout = nullptr;
	pVShader = nullptr;
	pPShader = nullptr;
	pVertexBuffer = nullptr;
}

Capp::~Capp()
{
	if (pDeviceContext)
	{
		pDeviceContext->ClearState();
	}
	if (pRenderTarget)
	{
		pRenderTarget->Release();
	}
	if (pSwapChain)
	{
		pSwapChain->Release();
	}
	if (pDeviceContext)
	{
		pDeviceContext->Release();
	}
	if (pDevice) 
	{
		pDevice->Release();
	}
	if (pLayout)
	{
		pLayout->Release();
	}
	if (pVShader)
	{
		pVShader->Release();
	}
	if (pPShader)
	{
		pPShader->Release();
	}
	if (pVertexBuffer)
	{
		pVertexBuffer->Release();
	}	
	if (Id_VShader)
	{
		Id_VShader->Release();
	}
	if (Id_PShader)
	{
		Id_PShader->Release();
	}
}

int Capp::Run()
{
	//Main Message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Render();
		}
	}

	return (int)msg.wParam;
}

bool Capp::Init_Window()
{
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = appinstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "CWindow";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wcex))
	{
		return false;
	}

	RECT R = { 0, 0, width, height };
	AdjustWindowRect(&R, wndS, FALSE);
	UINT width = R.right - R.left;
	UINT height = R.bottom - R.top;
	UINT x = GetSystemMetrics(SM_CXSCREEN)/2 - width/2;
	UINT y = GetSystemMetrics(SM_CXSCREEN) / 2 - height / 2;
	Ventana = CreateWindow("CWindow", titulo.c_str(), wndS, x, y, width, height, NULL, NULL, appinstance, NULL);
	if (!Ventana)
	{
		return false;
	}
	ShowWindow(Ventana, SW_SHOW);

	return true;
}

bool Capp::Init_Device()
{

	//swapchain
	
	memset(&swapchaindesc, NULL, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapchaindesc.BufferCount = 1;
	swapchaindesc.BufferDesc.Width = width;
	swapchaindesc.BufferDesc.Height = height;
	swapchaindesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchaindesc.BufferDesc.RefreshRate.Numerator = 60;
	swapchaindesc.BufferDesc.RefreshRate.Denominator = 1;
	swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapchaindesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapchaindesc.OutputWindow = Ventana;
	swapchaindesc.SampleDesc.Count = 1;
	swapchaindesc.SampleDesc.Quality = 0;
	swapchaindesc.Windowed = true;
	if (NULL == &swapchaindesc)
	{
		return false;
	}

	D3D11CreateDeviceAndSwapChain(NULL, DriverTypes, NULL, DFlags, NULL, NULL,
		D3D11_SDK_VERSION, &swapchaindesc, &pSwapChain, &pDevice, &FeatureLevel, &pDeviceContext);
	if (nullptr == pSwapChain && nullptr == pDevice)
	{
		return false;
	}

	pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBufferTextura);
	pDevice->CreateRenderTargetView(pBackBufferTextura, NULL, &pRenderTarget);
	if (nullptr == pRenderTarget)
	{
		return false;
	}
	pBackBufferTextura->Release();
	pDeviceContext->OMSetRenderTargets(1, &pRenderTarget, NULL);

	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;	
	pDeviceContext->RSSetViewports(1, &viewport);
	if (nullptr == pDeviceContext && NULL == &viewport)
	{
		return false;
	}
	return true;
}

bool Capp::Init_Triangle()
{
	//grafic pipeline
	D3DX11CompileFromFile("shader.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &Id_VShader, 0, 0);
	D3DX11CompileFromFile("shader.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &Id_PShader, 0, 0);
	if (nullptr == Id_PShader && nullptr == Id_VShader)
	{
		return false;
	}
	pDevice->CreateVertexShader(Id_VShader->GetBufferPointer(), Id_VShader->GetBufferSize(), NULL, &pVShader);
	pDevice->CreatePixelShader(Id_PShader->GetBufferPointer(), Id_PShader->GetBufferSize(), NULL, &pPShader);
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	pDevice->CreateInputLayout(ied, 2, Id_VShader->GetBufferPointer(), Id_VShader->GetBufferSize(), &pLayout);
	pDeviceContext->IASetInputLayout(pLayout);

    Vertex4 vertices[] =
    {
		/*{ 0.0f, 0.5f, 0.0f, c.red  },
		{ 0.45f, -0.5, 0.0f, c.green},
		{ -0.45f, -0.5f, 0.0f, c.blue}*/
		{ 0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) },
		{ 0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) },
		{ -0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) }
    };

	//initialize  vertex buffer
	memset(&vertexbufferdesc, NULL, sizeof(D3D11_BUFFER_DESC));
    vertexbufferdesc.Usage = D3D11_USAGE_DYNAMIC;                
    vertexbufferdesc.ByteWidth = sizeof(Vertex4) * 3;             
    vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       
    vertexbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;      

	//initialize data for triangle & CreateBuffer() 
	memset(&InitData, NULL ,sizeof(InitData) );
    InitData.pSysMem = vertices;
	pDevice->CreateBuffer(&vertexbufferdesc, &InitData, &pVertexBuffer);
	D3D11_MAPPED_SUBRESOURCE ms;
	pDeviceContext->Map(pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices, sizeof(vertices));                
	pDeviceContext->Unmap(pVertexBuffer, NULL);
	return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}

void Capp::Render()
{
	
}

bool Capp::Init()
{
	if (!Init_Window())
	{
		return false;
	}
	if (!Init_Device())
	{
		return false;
	}
	if (!Init_Triangle())
	{
		return false;
	}
	return true;	
}

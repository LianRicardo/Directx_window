#include "CappGraphix.h"

CappGraphix::CappGraphix(HINSTANCE instancia) : Capp(instancia)
{

}

CappGraphix::~CappGraphix()
{

}

bool CappGraphix::Init()
{
	if (!Capp::Init())
	{
		return false;
	}
	return true;
}

void CappGraphix::Render()
{
	pDeviceContext->ClearRenderTargetView(pRenderTarget, c.ligthblue);
	UINT stride = sizeof(Vertex4);
	UINT offset = 0;
	pDeviceContext->VSSetShader(pVShader, NULL, 0);
	pDeviceContext->PSSetShader(pPShader, NULL, 0);
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->Draw(3, 0);
	pSwapChain->Present(0, 0);
}

#include "Capp.h"
#include "CappGraphix.h"

int WINAPI WinMain( HINSTANCE Hinstance,  HINSTANCE Hpreviusinstance,  LPSTR lpcomandline,  int nshowcmd)
{
	CappGraphix app(Hinstance);
	if (!app.Init())
	{
		return -1;
	}
	return app.Run();
}
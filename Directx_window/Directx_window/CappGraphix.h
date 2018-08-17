#pragma once
#include "Capp.h"

class CappGraphix : public Capp
{
public:
	CappGraphix(HINSTANCE instancia);
	~CappGraphix(void);
	bool Init() override;
	void Render() override;
};

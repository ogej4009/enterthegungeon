#pragma once
#include "Scene.h"


class CMainScene :
	public CScene
{
public:
	CMainScene();
	~CMainScene();

public:
	virtual void Start();
	virtual bool Init();

private:
	void CreatePrototype();
};


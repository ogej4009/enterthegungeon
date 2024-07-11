#pragma once
#include "Scene.h"


class CStartScene :
	public CScene
{
public:
	CStartScene();
	~CStartScene();

public:
	virtual void Start();
	virtual bool Init();

public:
	void StartButton(float fTime);
	void EditButton(float fTime);
	void ExitButton(float fTime);
};

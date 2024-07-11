#pragma once
#include "Misc.h"


class CSolid :
	public CMisc
{
public:
	CSolid();
	CSolid(const CSolid& obj);
	virtual ~CSolid();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(HDC hDC, float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual void PostRender(HDC hDC, float fTime);
	virtual CMisc* Clone()	const;

};

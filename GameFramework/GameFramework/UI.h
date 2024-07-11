#pragma once
#include "Obj.h"


class CUI :
	public CObj
{
	DECLARE_OBJECT

protected:
	CUI();
	CUI(const CUI& obj);
	virtual ~CUI();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(HDC hDC, float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual void PostRender(HDC hDC, float fTime);
	virtual CUI* Clone()	const;
};

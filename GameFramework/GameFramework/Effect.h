#pragma once
#include "Obj.h"


class CEffect :
	public CObj
{
	DECLARE_OBJECT

protected:
	CEffect();
	CEffect(const CEffect& obj);
	virtual ~CEffect();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(HDC hDC, float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual void PostRender(HDC hDC, float fTime);
	virtual CEffect* Clone()	const;
};

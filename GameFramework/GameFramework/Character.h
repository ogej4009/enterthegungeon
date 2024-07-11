#pragma once
#include "Obj.h"
#include "InputComponent.h"


class CCharacter :
	public CObj
{
protected:
	CCharacter();
	CCharacter(const CCharacter& obj);
	virtual ~CCharacter() = 0;

protected:
	CInputComponent* m_pInputComponent;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(HDC hDC, float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual void PostRender(HDC hDC, float fTime);
	virtual CCharacter* Clone()	const;
};


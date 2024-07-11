#pragma once
#include "UI.h"


class CStartImage :
	public CUI
{

public:
	CStartImage();
	CStartImage(const CStartImage& obj);
	virtual ~CStartImage();

public:
	class CTexture* m_pTexture;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(HDC hDC, float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual void PostRender(HDC hDC, float fTime);
	virtual CStartImage* Clone()	const;
};


#pragma once
#include "UI.h"


class CUIBar :
	public CUI
{
	DECLARE_OBJECT

protected:
	CUIBar();
	CUIBar(const CUIBar& obj);
	~CUIBar();

protected:
	float	m_fMin;
	float	m_fMax;
	float	m_fValue;

public:
	void SetMin(float fMin)
	{
		m_fMin = fMin;
	}

	void SetMax(float fMax)
	{
		m_fMax = fMax;
	}

	void SetValue(float fValue)
	{
		m_fValue = fValue;
	}

	void AddValue(float fValue)
	{
		m_fValue += fValue;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(HDC hDC, float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual void PostRender(HDC hDC, float fTime);
	virtual CUIBar* Clone()	const;
};


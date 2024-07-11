#pragma once
#include "UI.h"


enum BUTTON_STATE
{
	BS_DISENABLE,
	BS_NORMAL,
	BS_MOUSEON,
	BS_CLICK,
	BS_END
};

class CUIButton :
	public CUI
{
	DECLARE_OBJECT

protected:
	CUIButton();
	CUIButton(const CUIButton& obj);
	virtual ~CUIButton();

protected:
	BUTTON_STATE	m_eState;
	BUTTON_STATE	m_ePrevState;
	class CTexture* m_pStateTexture[BS_END];
	bool			m_bMouseOn;
	function<void(float)>	m_StateCallback[BS_END];

public:
	void DisEnable(bool bDisEnable)
	{
		if (bDisEnable)
			m_eState = BS_DISENABLE;

		else
			m_eState = BS_NORMAL;

		if (m_pStateTexture[m_eState])
			SetTexture(m_pStateTexture[m_eState]);
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
	virtual CUIButton* Clone()	const;

public:
	void Hit(CCollider* pSrc, CCollider* pDest, float fTime); // 클릭이 가능한 상태 
	void HitRelease(CCollider* pSrc, CCollider* pDest, float fTime); // 클릭이 불가능한 상태 


public:
	template <typename T>
	void SetButtonStateCallback(BUTTON_STATE eState,
		T* pObj, void (T::* pFunc)(float))
	{
		m_StateCallback[eState] = bind(pFunc, pObj, placeholders::_1);
	}
};


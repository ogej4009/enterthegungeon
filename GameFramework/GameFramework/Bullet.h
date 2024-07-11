#pragma once
#include "Obj.h"

class CCollider;
class CBullet :
	public CObj
{
	DECLARE_OBJECT

protected:
	CBullet();
	CBullet(const CBullet& obj);
	virtual ~CBullet();

private:
	MOVE_DIR	m_eDir;
	CObj* m_pTarget;

public:
	void SetMoveDir(MOVE_DIR eDir)
	{
		m_eDir = eDir;
	}

	void SetTarget(CObj* pTarget)
	{
		if (pTarget)
			pTarget->AddRef();

		m_pTarget = pTarget;
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
	virtual CBullet* Clone()	const;

public:
	void Hit(CCollider* pSrc, CCollider* pDest, float fTime);
};


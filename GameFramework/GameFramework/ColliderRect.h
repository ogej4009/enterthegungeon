#pragma once
#include "Collider.h"


class CColliderRect :
	public CCollider
{
	friend class CObj;

private:
	CColliderRect();
	CColliderRect(const CColliderRect& collider);
	~CColliderRect();

private:
	RectInfo	m_tRelative;
	RectInfo	m_tInfo;

public:
	void SetRelativeInfo(float l, float t, float r, float b)
	{
		m_tRelative.l = l;
		m_tRelative.t = t;
		m_tRelative.r = r;
		m_tRelative.b = b;
	}

public:
	RectInfo GetInfo()	const
	{
		return m_tInfo;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void LateUpdate(float fTime);
	virtual void PrevRender(HDC hDC, float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual void PostRender(HDC hDC, float fTime);
	virtual CColliderRect* Clone()	const;
	virtual bool Collision(CCollider* pCollider);
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual void UpdateObjPos(const Position& tPos);
};


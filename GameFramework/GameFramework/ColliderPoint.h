#pragma once
#include "Collider.h"


class CColliderPoint :
	public CCollider
{
	friend class CObj;

private:
	CColliderPoint();
	CColliderPoint(const CColliderPoint& collider);
	~CColliderPoint();

private:
	Position	m_tRelative;
	Position	m_tInfo;

public:
	void SetRelativeInfo(const Position& tPos)
	{
		m_tRelative = tPos;
	}

	void SetRelativeInfo(float x, float y)
	{
		m_tRelative = Position(x, y);
	}

public:
	Position GetInfo()	const
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
	virtual CColliderPoint* Clone()	const;
	virtual bool Collision(CCollider* pCollider);
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual void UpdateObjPos(const Position& tPos);
};


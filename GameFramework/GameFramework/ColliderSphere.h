#pragma once
#include "Collider.h"


class CColliderSphere :
	public CCollider
{
	friend class CObj;

private:
	CColliderSphere();
	CColliderSphere(const CColliderSphere& collider);
	~CColliderSphere();

private:
	SphereInfo	m_tRelative;
	SphereInfo	m_tInfo;

public:
	void SetRelativeInfo(const Position& tCenter, float fRadius)
	{
		m_tRelative.tCenter = tCenter;
		m_tRelative.fRadius = fRadius;

		m_tInfo.fRadius = fRadius;
	}

	void SetRelativeInfo(float x, float y, float fRadius)
	{
		m_tRelative.tCenter = Position(x, y);
		m_tRelative.fRadius = fRadius;

		m_tInfo.fRadius = fRadius;
	}

public:
	SphereInfo GetInfo()	const
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
	virtual CColliderSphere* Clone()	const;
	virtual bool Collision(CCollider* pCollider);
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual void UpdateObjPos(const Position& tPos);
	/*
		오브젝트 위치정보를 갱신할 때
		충돌체들의 위치에까지 영향을 주게 만들어준 함수
	*/
};


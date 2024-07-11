#pragma once
#include "Character.h"


class CMonster :
	public CCharacter
{
	DECLARE_OBJECT

protected:
	CMonster();
	CMonster(const CMonster& obj);
	virtual ~CMonster();

private:
	MOVE_DIR	m_eDir;
	float		m_fFireTime;
	float		m_fFireTimeLimit;
	CObj* m_pTarget;

public:
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
	virtual CMonster* Clone()	const;


private:
	void CreateClip();

};


#pragma once
#include "Character.h"


class CPlayer :
	public CCharacter
{
	DECLARE_OBJECT

protected:
	CPlayer();
	CPlayer(const CPlayer& obj);
	virtual ~CPlayer();

private:
	SIDE_DIR	m_eSideDir;
	int			m_iHP;
	int			m_iHPMax;

private:
	vector<function<void(float)>>	m_vecHPCallback;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(HDC hDC, float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual void PostRender(HDC hDC, float fTime);
	virtual CPlayer* Clone()	const;

public:
	void MoveFront(float fScale, float fTime);
	void MoveSide(float fScale, float fTime);
	void Dash(float fTime);
	void Fire(float fTime);
	void Skill1(float fTime);
	void Skill2(float fTime);

public:
	void Hit(CCollider* pSrc, CCollider* pDest, float fTime);

private:
	void CreateClip();

public:
	template <typename T>
	void AddHPCallback(T* pObj, void (T::* pFunc)(float))
	{
		function<void(float)>	func;
		func = bind(pFunc, pObj, placeholders::_1);
		m_vecHPCallback.push_back(func);
	}
};


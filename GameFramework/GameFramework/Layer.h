#pragma once
#include "Ref.h"

class CLayer :
	public CRef
{
	friend class CSceneManager;

protected:
	CLayer();
	~CLayer();

private:
	class CScene* m_pScene;
	list<class CObj*>	m_ObjList;
	int		m_iZOrder;

public:
	void AddObject(class CObj* pObj);
	void DeleteObject(class CScene* pScene);

	void SetScene(class CScene* pScene)
	{
		m_pScene = pScene;
	}

	void SetZOrder(int iZOrder)
	{
		m_iZOrder = iZOrder;
	}

	int GetZOrder()	const
	{
		return m_iZOrder;
	}

public:
	void Start();
	bool Init();
	void Update(float fTime);
	void LateUpdate(float fTime);
	void Collision(float fTime);
	void PrevRender(HDC hDC, float fTime);
	void Render(HDC hDC, float fTime);
	void PostRender(HDC hDC, float fTime);
};


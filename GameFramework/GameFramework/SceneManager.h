#pragma once
#include "Game.h"


class CSceneManager
{
private:
	class CScene* m_pScene;
	class CScene* m_pNextScene;
	list<class CLayer*>	m_LayerList;
	bool			m_bCreateScene;

public:
	void CreateScene(bool bScene)
	{
		m_bCreateScene = bScene;
	}

	bool CreateSceneEnable()
	{
		return m_bCreateScene;
	}

	class CScene* GetScene()	const
	{
		return m_pScene;
	}

public:
	bool Init();
	int Update(float fTime);
	int LateUpdate(float fTime);
	int Collision(float fTime);
	void PrevRender(HDC hDC, float fTime);
	void Render(HDC hDC, float fTime);
	void PostRender(HDC hDC, float fTime);

private:
	int ChangeScene();

public:
	void AddLayer(const string& strName, int iZOrder = 0);
	void SetLayerZOrder(const string& strName, int iZOrder = 0);
	class CLayer* FindLayer(const string& strName);

public:
	template <typename T>
	bool CreateScene(const string& strName, bool bCurrent = false)
	{
		T* pScene = new T;

		m_bCreateScene = true;

		pScene->SetLayerList(&m_LayerList);
		pScene->SetTag(strName);

		if (!pScene->Init())
		{
			SAFE_DELETE(pScene);
			return false;
		}

		if (bCurrent)
			m_pScene = pScene;

		else
			m_pNextScene = pScene;

		m_bCreateScene = false;

		return true;
	}

private:
	static bool SortLayer(class CLayer* pSrc, class CLayer* pDest);

	DECLARE_SINGLE(CSceneManager)
};


#include "Scene.h"
#include "Layer.h"
#include "Obj.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"


CScene::CScene()
{
	m_pLayerList = nullptr;
	m_pStage = nullptr;
}

CScene::~CScene()
{
	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_pLayerList->end();

	for (iter = m_pLayerList->begin(); iter != iterEnd; ++iter)
	{
		(*iter)->DeleteObject(this);
	}

	Safe_Release_Map(m_mapPrototype);
}

void CScene::AddPrototype(CObj* pObj)
{
	pObj->SetScene(this);
	pObj->AddRef();
	m_mapPrototype.insert(make_pair(pObj->GetTag(), pObj));
}

CObj* CScene::ClonePrototype(const string& strProtoName,
	const string& strName)
{
	CObj* pPrototype = FindPrototype(strProtoName);

	if (!pPrototype)
		return nullptr;

	CObj* pObj = pPrototype->Clone();

	pObj->SetTag(strName);

	return pObj;
}

CObj* CScene::FindPrototype(const string& strName)
{
	unordered_map<string, CObj*>::iterator	iter = m_mapPrototype.find(strName);

	if (iter == m_mapPrototype.end())
		return nullptr;

	return iter->second;
}

bool CScene::DeletePrototype(const string& strName)
{
	unordered_map<string, CObj*>::iterator	iter = m_mapPrototype.find(strName);

	if (iter == m_mapPrototype.end())
		return false;

	m_mapPrototype.erase(strName);

	return true;
}

void CScene::SetLayerList(list<CLayer*>* pLayerList)
{
	m_pLayerList = pLayerList;

	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = pLayerList->end();

	for (iter = pLayerList->begin(); iter != iterEnd; ++iter)
	{
		(*iter)->SetScene(this);
	}
}

void CScene::AddObject(CObj* pObj)
{
	pObj->SetScene(this);
	m_ObjList.push_back(pObj);
	m_WorldObjList.push_back(pObj);
}

void CScene::DeleteObject(CObj* pObj)
{
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pObj)
		{
			m_ObjList.erase(iter);
			break;
		}
	}
}

CObj* CScene::FindObject(const string& strTag)
{
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return nullptr;
}

void CScene::SetWorld()
{
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_WorldObjList.end();

	for (iter = m_WorldObjList.begin(); iter != iterEnd; ++iter)
	{
		CLayer* pLayer = GET_SINGLE(CSceneManager)->FindLayer((*iter)->GetLayerName());

		if (pLayer)
		{
			(*iter)->Start();
			pLayer->AddObject(*iter);
		}

		SAFE_RELEASE(pLayer);
	}

	m_WorldObjList.clear();
}

void CScene::Start()
{
	SetWorld();

	GET_SINGLE(CResourceManager)->ClearScene();
	GET_SINGLE(CSoundManager)->ClearScene();
}

bool CScene::Init()
{
	return true;
}

void CScene::Update(float fTime)
{

	// 월드를 세팅한다. 
	if (!m_WorldObjList.empty())
		SetWorld();

	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_pLayerList->end();

	for (iter = m_pLayerList->begin(); iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_pLayerList->erase(iter);
			iterEnd = m_pLayerList->end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(fTime);
		++iter;
	}
}

void CScene::LateUpdate(float fTime)
{
	if (!m_WorldObjList.empty())
		SetWorld();

	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_pLayerList->end();

	for (iter = m_pLayerList->begin(); iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_pLayerList->erase(iter);
			iterEnd = m_pLayerList->end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->LateUpdate(fTime);
		++iter;
	}
}

void CScene::Collision(float fTime)
{
	if (!m_WorldObjList.empty())
		SetWorld();

	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_pLayerList->end();

	for (iter = m_pLayerList->begin(); iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_pLayerList->erase(iter);
			iterEnd = m_pLayerList->end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Collision(fTime);
		++iter;
	}
}

void CScene::PrevRender(HDC hDC, float fTime)
{
	if (!m_WorldObjList.empty())
		SetWorld();

	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_pLayerList->end();

	for (iter = m_pLayerList->begin(); iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_pLayerList->erase(iter);
			iterEnd = m_pLayerList->end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PrevRender(hDC, fTime);
		++iter;
	}
}

void CScene::Render(HDC hDC, float fTime)
{
	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_pLayerList->end();

	for (iter = m_pLayerList->begin(); iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_pLayerList->erase(iter);
			iterEnd = m_pLayerList->end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render(hDC, fTime);
		++iter;
	}
}

void CScene::PostRender(HDC hDC, float fTime)
{
	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_pLayerList->end();

	for (iter = m_pLayerList->begin(); iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_pLayerList->erase(iter);
			iterEnd = m_pLayerList->end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostRender(hDC, fTime);
		++iter;
	}
}

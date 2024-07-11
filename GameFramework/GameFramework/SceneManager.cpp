#include "SceneManager.h"
#include "MainScene.h"
#include "StartScene.h"
#include "Layer.h"
#include "CollisionManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "Camera.h"
#include "Input.h" // ��


DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager() :
	m_pScene(nullptr),
	m_pNextScene(nullptr),
	m_bCreateScene(false)
{
}

CSceneManager::~CSceneManager()
{
	GET_SINGLE(CCollisionManager)->ClearWorldList();
	Safe_Release_VecList(m_LayerList);

	SAFE_RELEASE(m_pScene);
	SAFE_RELEASE(m_pNextScene);
}

bool CSceneManager::Init()
{
	// �⺻ �����ϴ� ���̾ �����Ѵ�.
	AddLayer("MISC", 5);
	AddLayer("Default", 1);
	AddLayer("BackGround");
	AddLayer("UI", INT_MAX - 1);
	AddLayer("HUD", INT_MAX);

	// �⺻ ����� �����Ѵ�.
	CreateScene<CStartScene>("StartScene", true);

	m_pScene->Start();

	return true;
}

int CSceneManager::Update(float fTime)
{
	m_pScene->Update(fTime);

	return ChangeScene();
}

int CSceneManager::LateUpdate(float fTime)
{
	m_pScene->LateUpdate(fTime);

	return ChangeScene();
}

int CSceneManager::Collision(float fTime)
{
	m_pScene->Collision(fTime);

	return ChangeScene();
}

void CSceneManager::PrevRender(HDC hDC, float fTime)
{
	m_pScene->PrevRender(hDC, fTime);
}

void CSceneManager::Render(HDC hDC, float fTime)
{
	m_pScene->Render(hDC, fTime);
}

void CSceneManager::PostRender(HDC hDC, float fTime)
{
	m_pScene->PostRender(hDC, fTime);
}

int CSceneManager::ChangeScene()
{
	if (m_pNextScene)
	{
		GET_SINGLE(CCollisionManager)->ChangeScene();
		GET_SINGLE(CInput)->ChangeScene(); // ��

		// ���� Scene�� �����Ѵ�.
		SAFE_RELEASE(m_pScene);

		GET_SINGLE(CResourceManager)->ClearScene();
		GET_SINGLE(CSoundManager)->ClearScene();

		m_pScene = m_pNextScene;
		m_pNextScene = nullptr;

		GET_SINGLE(CCamera)->SetSceneSize(m_pScene->GetSceneSize());

		return SC_CHANGE;
	}

	return SC_NONE;
}

void CSceneManager::AddLayer(const string& strName, int iZOrder)
{
	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strName)
		{
			return;
		}
	}

	CLayer* pLayer = new CLayer;

	if (!pLayer->Init())
	{
		SAFE_RELEASE(pLayer);
		return;
	}

	pLayer->SetTag(strName);
	pLayer->SetZOrder(iZOrder);

	m_LayerList.push_back(pLayer);

	// �߰��� ���̾ 2�� �̻��� ��� �߰��ɶ�����
	// ������ ���ش�.
	if (m_LayerList.size() >= 2)
		m_LayerList.sort(CSceneManager::SortLayer);
}

void CSceneManager::SetLayerZOrder(const string& strName, int iZOrder)
{
	CLayer* pLayer = FindLayer(strName);

	if (!pLayer)
		return;

	pLayer->SetZOrder(iZOrder);

	SAFE_RELEASE(pLayer);

	// �߰��� ���̾ 2�� �̻��� ��� �߰��ɶ�����
	// ������ ���ش�.
	m_LayerList.sort(CSceneManager::SortLayer);
}

CLayer* CSceneManager::FindLayer(const string& strName)
{
	list<CLayer*>::iterator	iter;
	list<CLayer*>::iterator	iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strName)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return nullptr;
}

bool CSceneManager::SortLayer(CLayer* pSrc, CLayer* pDest)
{
	return pSrc->GetZOrder() < pDest->GetZOrder();
}

#include "ResourceManager.h"
#include "GameManager.h"
#include "Texture.h"
#include "SceneManager.h"



DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
	Safe_Release_Map(m_mapTexture);
}

bool CResourceManager::Init()
{
	m_hInst = WINDOWINSTANCE;
	m_hDC = WINDOWDC;
	m_hWnd = WINDOWHANDLE;

	// BackBuffer를 미리 세팅한다. ★
	LoadTexture("BackBuffer", TEXT("BackBuffer.bmp"));
	int a = 0;
	return true;
}

// 이미지가 아틀라스 형으로 파일 하나로 모든 처리를 하기 때문에  pFileName만 넘어온다.
// 이 로드텍스쳐는 한번에 LoadTexture를 호출하는 함수
bool CResourceManager::LoadTexture(const string& strName,
	const TCHAR* pFileName, const string& strPathName)
{
	// 이미지 생성시 카운트를 2로 해놓고 장면 전환시
	// 원하는 이미지들만 지워지도록 처리한다.
	CTexture* pTexture = FindTexture(strName);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	pTexture = new CTexture;

	if (!pTexture->LoadTexture(strName, pFileName, strPathName))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	if (GET_SINGLE(CSceneManager)->CreateSceneEnable())
		pTexture->AddRef();

	m_mapTexture.insert(make_pair(strName, pTexture));

	return true;
}

bool CResourceManager::LoadTexture(const string& strName,
	const vector<const TCHAR*>& vecFileName, const string& strPathName)
{
	/*
		이미지가 프레임 단위로 파일이 여러 개 이기 때문에 vecFileName이 넘어온다.
		이 로드 텍스쳐는 vecFileName을 넘긴다. 그리고 하나씩 반복문으로 목록을 탐색하면서 LoadTexture를 호출한다.
		(한번 더 거쳐서 한다)
	*/

	// 이미지 생성시 카운트를 2로 해놓고 장면 전환시
	// 원하는 이미지들만 지워지도록 처리한다.
	CTexture* pTexture = FindTexture(strName);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	pTexture = new CTexture;

	if (!pTexture->LoadTexture(strName, vecFileName, strPathName))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	if (GET_SINGLE(CSceneManager)->CreateSceneEnable())
		pTexture->AddRef();

	m_mapTexture.insert(make_pair(strName, pTexture));

	return true;
}

void CResourceManager::ClearScene()
{
	// ClearScene 
	// LoadTexture("BackBuffer", TEXT("BackBuffer.bmp")); 백버퍼의 텍스쳐를 가져온다. 리소스매니저  init에서 일어난 일 
	// m_pBackBuffer = GET_SINGLE(CResourceManager)->FindTexture("BackBuffer"); 게임매니저 init에서 일어난 일 
	// ★
	FindTexture("BackBuffer");// FindTexture는 참조카운터를 올린다.

	unordered_map<string, class CTexture*>::iterator	iter;
	unordered_map<string, class CTexture*>::iterator	iterEnd = m_mapTexture.end();

	for (iter = m_mapTexture.begin(); iter != iterEnd;)
	{
		if (iter->second->Release() == 0)
		{
			iter = m_mapTexture.erase(iter);
			iterEnd = m_mapTexture.end();
			continue;
		}

		++iter;
	}
}

CTexture* CResourceManager::FindTexture(
	const string& strName)
{
	unordered_map<string, class CTexture*>::iterator	iter = m_mapTexture.find(strName);

	if (iter == m_mapTexture.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

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

	// BackBuffer�� �̸� �����Ѵ�. ��
	LoadTexture("BackBuffer", TEXT("BackBuffer.bmp"));
	int a = 0;
	return true;
}

// �̹����� ��Ʋ�� ������ ���� �ϳ��� ��� ó���� �ϱ� ������  pFileName�� �Ѿ�´�.
// �� �ε��ؽ��Ĵ� �ѹ��� LoadTexture�� ȣ���ϴ� �Լ�
bool CResourceManager::LoadTexture(const string& strName,
	const TCHAR* pFileName, const string& strPathName)
{
	// �̹��� ������ ī��Ʈ�� 2�� �س��� ��� ��ȯ��
	// ���ϴ� �̹����鸸 ���������� ó���Ѵ�.
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
		�̹����� ������ ������ ������ ���� �� �̱� ������ vecFileName�� �Ѿ�´�.
		�� �ε� �ؽ��Ĵ� vecFileName�� �ѱ��. �׸��� �ϳ��� �ݺ������� ����� Ž���ϸ鼭 LoadTexture�� ȣ���Ѵ�.
		(�ѹ� �� ���ļ� �Ѵ�)
	*/

	// �̹��� ������ ī��Ʈ�� 2�� �س��� ��� ��ȯ��
	// ���ϴ� �̹����鸸 ���������� ó���Ѵ�.
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
	// LoadTexture("BackBuffer", TEXT("BackBuffer.bmp")); ������� �ؽ��ĸ� �����´�. ���ҽ��Ŵ���  init���� �Ͼ �� 
	// m_pBackBuffer = GET_SINGLE(CResourceManager)->FindTexture("BackBuffer"); ���ӸŴ��� init���� �Ͼ �� 
	// ��
	FindTexture("BackBuffer");// FindTexture�� ����ī���͸� �ø���.

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

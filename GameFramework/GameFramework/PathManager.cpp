#include "PathManager.h"
#include "string"

DEFINITION_SINGLE(CPathManager)

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
	Safe_Delete_Array_Map(m_mapPath);
}

bool CPathManager::Init()
{
	// TCHAR : �����ڵ�/��Ƽ����Ʈ �ڵ���ȯ 
	// ���ڿ� �޸� ���� : ���ڿ� �ּ� �Ҵ� 
	// ���ڿ��� �޾� ���� 0���� �ʱ�ȭ�Ѵ�. 
	TCHAR* pPath = new TCHAR[MAX_PATH];
	memset(pPath, 0, sizeof(TCHAR) * MAX_PATH);
	
	// WIN32���� ��θ� �������� �Լ� pPath�� ������ ���� 
	// win 32�� �Լ��̴� : ( h��� , ���ڿ� , ���� ��Ʈ�� )  
	GetModuleFileName(nullptr, pPath, MAX_PATH); // MAX_PATH : 260

	// �������� �̸��� �����Ѵ�.
	// ���ڿ��� ���̸� ���´�.
	int	iLength = lstrlen(pPath);

	//���� ���� �̸� ����(��� ���)
	for (int i = iLength - 1; i >= 0; --i)
	{
		// aa/bb
		// ���� ��� ��ü 2���ε����� �ڸ� 0���� 
		// �ٲ���� �Ѵ�.
		// 3�� �ε������� 4�������� �ٲٰ� �Ǵµ�
		// ��ü ���̰� 5�̹Ƿ� 3�� �ε������� 5 - 3���ε���
		// (2����ŭ) 0���� �ʱ�ȭ�Ѵ�.

		/*if (pPath[i] == '\\')
		{
			memset(&pPath[i + 1], 0, sizeof(TCHAR) * (iLength - (i + 1)));
			int cc = 0;
			break;
		}*/

		/// ���ļ��� 
		if (pPath[i] == 'b') // bin
		{
			memset(&pPath[i], 0, sizeof(TCHAR) * (iLength - (i + 1)));
			int cc = 0;
			break;
		}

	}
	int bb = 0;

	// ��ε��� ������ STL map�� �ڵ����� ���� ���缭 ROOT_PATH�̸����� �߰����ش�.
	m_mapPath.insert(make_pair(ROOT_PATH, pPath));


	// �� ��� �߰� 
	AddPath(TEXTURE_PATH, TEXT("Res\\Texture\\"));
	AddPath(DATA_PATH, TEXT("Res\\Data\\"));
	AddPath(SOUND_PATH, TEXT("Res\\Sound\\"));

	return true;
}

void CPathManager::AddPath(const string& strName,
	const TCHAR* pPath, const string& strBasePath)
{
	if (FindPath(strName))
		return;

	const TCHAR* pBasePath = FindPath(strBasePath);

	TCHAR* pFullPath = new TCHAR[MAX_PATH];
	memset(pFullPath, 0, sizeof(TCHAR) * MAX_PATH);

	if (pBasePath)
		lstrcpy(pFullPath, pBasePath);

	lstrcat(pFullPath, pPath);

	m_mapPath.insert(make_pair(strName, pFullPath));
} 

// �����ڵ� 
const TCHAR* CPathManager::FindPath(const string& strName)
{
	unordered_map<string, TCHAR*>::iterator	iter = m_mapPath.find(strName);

	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second;
}

// ��Ƽ����Ʈ 
const char* CPathManager::FindPathMultibyte(const string& strName)
{
	const TCHAR* pPath = FindPath(strName);

	if (!pPath)
		return nullptr;

	memset(m_strMultibyte, 0, sizeof(char) * MAX_PATH);

	// �ٲ� ���̸� ���´�.
	// WideCharToMultiByte : 1����Ʈ ���ڿ��� ��Ƽ����Ʈ�� �ٲ��ش�.
	int iLength = WideCharToMultiByte(CP_ACP, 0, pPath,
		-1, nullptr, 0,
		nullptr, nullptr);

	// ������ ���� ���̸� �̿��ؼ� ���ڿ��� ��ȯ�Ѵ�.
	WideCharToMultiByte(CP_ACP, 0, pPath, -1, m_strMultibyte,
		iLength, nullptr, nullptr);
	/*
		�ּ� �ѱ� ��, ���������� �ּҸ� �ѱ�� �ȵȴ�.
		m_strMultibyte ��������� ��Ƴ��� ������ �ִ�.
	*/
	return m_strMultibyte;
}

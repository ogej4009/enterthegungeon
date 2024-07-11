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
	// TCHAR : 유니코드/멀티바이트 자동변환 
	// 문자열 메모리 생성 : 문자열 주소 할당 
	// 문자열을 받아 값을 0으로 초기화한다. 
	TCHAR* pPath = new TCHAR[MAX_PATH];
	memset(pPath, 0, sizeof(TCHAR) * MAX_PATH);
	
	// WIN32에서 경로를 가져오는 함수 pPath에 값으로 지정 
	// win 32의 함수이다 : ( h모듈 , 문자열 , 설정 매트로 )  
	GetModuleFileName(nullptr, pPath, MAX_PATH); // MAX_PATH : 260

	// 실행파일 이름을 제거한다.
	// 문자열의 길이를 얻어온다.
	int	iLength = lstrlen(pPath);

	//실행 파일 이름 제거(경로 취득)
	for (int i = iLength - 1; i >= 0; --i)
	{
		// aa/bb
		// 위의 경우 전체 2번인덱스의 뒤를 0으로 
		// 바꿔줘야 한다.
		// 3번 인덱스부터 4번까지를 바꾸게 되는데
		// 전체 길이가 5이므로 3번 인덱스부터 5 - 3번인덱스
		// (2개만큼) 0으로 초기화한다.

		/*if (pPath[i] == '\\')
		{
			memset(&pPath[i + 1], 0, sizeof(TCHAR) * (iLength - (i + 1)));
			int cc = 0;
			break;
		}*/

		/// 차후수정 
		if (pPath[i] == 'b') // bin
		{
			memset(&pPath[i], 0, sizeof(TCHAR) * (iLength - (i + 1)));
			int cc = 0;
			break;
		}

	}
	int bb = 0;

	// 경로들을 저장한 STL map에 자동으로 형식 맞춰서 ROOT_PATH이름으로 추가해준다.
	m_mapPath.insert(make_pair(ROOT_PATH, pPath));


	// ★ 경로 추가 
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

// 유니코드 
const TCHAR* CPathManager::FindPath(const string& strName)
{
	unordered_map<string, TCHAR*>::iterator	iter = m_mapPath.find(strName);

	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second;
}

// 멀티바이트 
const char* CPathManager::FindPathMultibyte(const string& strName)
{
	const TCHAR* pPath = FindPath(strName);

	if (!pPath)
		return nullptr;

	memset(m_strMultibyte, 0, sizeof(char) * MAX_PATH);

	// 바꿀 길이를 얻어온다.
	// WideCharToMultiByte : 1바이트 문자열을 멀티바이트로 바꿔준다.
	int iLength = WideCharToMultiByte(CP_ACP, 0, pPath,
		-1, nullptr, 0,
		nullptr, nullptr);

	// 위에서 얻어온 길이를 이용해서 문자열을 변환한다.
	WideCharToMultiByte(CP_ACP, 0, pPath, -1, m_strMultibyte,
		iLength, nullptr, nullptr);
	/*
		주소 넘길 때, 지역변수의 주소를 넘기면 안된다.
		m_strMultibyte 멤버변수로 잡아놓은 이유가 있다.
	*/
	return m_strMultibyte;
}

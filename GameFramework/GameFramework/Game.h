#pragma once

#if defined(_DEBUG)
#define	_CRTDBG_MAP_ALLOC
#define	new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// 사용자정의 헤더파일
#include "Macro.h"
#include "Types.h"
#include "resource.h"


// Path Key
#define	ROOT_PATH		"RootPath"
#define	TEXTURE_PATH	"TexturePath"
#define	DATA_PATH		"DataPath"
#define	SOUND_PATH		"SoundPath"

// 메모리를 지우는 함수 
// 컨테이너의 어떤 타입이 들어올지 알수 없다. 
template <typename T>
void Safe_Delete_VecList(T& container)
{
	// typename을 항상 붙여준다. : 템플릿을 컴파일러가 해석하는 방식이 다르다 2017에서는 에러 
	typename T::iterator	iter;
	typename T::iterator	iterEnd = container.end();

	for (iter = container.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}
	// STL clear() 
	container.clear();
}

template <typename T>
void Safe_Delete_Map(T& container)
{
	typename T::iterator	iter;
	typename T::iterator	iterEnd = container.end();

	for (iter = container.begin(); iter != iterEnd; ++iter)
	{
		// BindAxisInfo 구조체 전부를 지운다
		SAFE_DELETE(iter->second);
	}
	// STL clear() 
	container.clear();
}

template <typename T>
void Safe_Delete_Array_VecList(T& container)
{
	typename T::iterator	iter;
	typename T::iterator	iterEnd = container.end();

	for (iter = container.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE_ARRAY((*iter));
	}
	// STL clear() 
	container.clear();
}

template <typename T>
void Safe_Delete_Array_Map(T& container)
{
	typename T::iterator	iter;
	typename T::iterator	iterEnd = container.end();

	for (iter = container.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE_ARRAY(iter->second);
	}
	// STL clear() 
	container.clear();
}

template <typename T>
void Safe_Release_VecList(T& container)// m_LayerList
{
	typename T::iterator	iter;
	typename T::iterator	iterEnd = container.end();

	for (iter = container.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE((*iter));
	}
	// list<class CLayer*>
	// m_LayerList.clear() : 자료구조 
	container.clear();
}

template <typename T>
void Safe_Release_Map(T& container)
{
	typename T::iterator	iter;
	typename T::iterator	iterEnd = container.end();

	for (iter = container.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second);
	}
	// STL clear() 
	container.clear();
}


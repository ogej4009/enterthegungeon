#pragma once

#if defined(_DEBUG)
#define	_CRTDBG_MAP_ALLOC
#define	new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// ��������� �������
#include "Macro.h"
#include "Types.h"
#include "resource.h"


// Path Key
#define	ROOT_PATH		"RootPath"
#define	TEXTURE_PATH	"TexturePath"
#define	DATA_PATH		"DataPath"
#define	SOUND_PATH		"SoundPath"

// �޸𸮸� ����� �Լ� 
// �����̳��� � Ÿ���� ������ �˼� ����. 
template <typename T>
void Safe_Delete_VecList(T& container)
{
	// typename�� �׻� �ٿ��ش�. : ���ø��� �����Ϸ��� �ؼ��ϴ� ����� �ٸ��� 2017������ ���� 
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
		// BindAxisInfo ����ü ���θ� �����
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
	// m_LayerList.clear() : �ڷᱸ�� 
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


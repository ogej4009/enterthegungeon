#pragma once
#include "Ref.h"
#include "Input.h"

typedef struct _tagAddFunction
{
	string	strName; // 디버깅 태그 
	void* pObj; // 해당 오브젝트  
	string	strFuncName; // 기능 함수이름 

	_tagAddFunction()
	{
		pObj = nullptr;
	}
}AddFunction, * PAddFunction;

class CInputComponent :
	public CRef
{
	// 컴포넌트를 사용하는 클래스 목록 
	friend class CInput;
	friend class CCharacter;
	friend class CCamera;
	friend class CEditScene;

private:
	CInputComponent();
	~CInputComponent();

private:
	list<AddFunction>	m_FunctionAxisList;
	list<AddFunction>	m_FunctionActionList;


public:
	template <typename T>
	void BindAxis(const string& strName, T* pObj,
		void (T::* pFunc)(float, float),
		const string& strFuncName)
	{
		// 컴포넌트를 한번 거쳐서 BindAxis을 호출한다. 
		if (GET_SINGLE(CInput)->BindAxis(strName, pObj, pFunc, strFuncName))
		{
			AddFunction	tFunc;

			tFunc.strName = strName;
			tFunc.pObj = pObj;
			tFunc.strFuncName = strFuncName;

			m_FunctionAxisList.push_back(tFunc);
		}
	}

	template <typename T>
	void BindAction(const string& strName,
		INPUT_EVENT eEvent, T* pObj,
		void (T::* pFunc)(float),
		const string& strFuncName)
	{
		// 컴포넌트를 한번 거쳐서 BindAction을 호출한다.  
		if (GET_SINGLE(CInput)->BindAction(strName, eEvent,
			pObj, pFunc, strFuncName))
		{
			AddFunction	tFunc;

			tFunc.strName = strName;
			tFunc.pObj = pObj;
			tFunc.strFuncName = strFuncName;

			m_FunctionActionList.push_back(tFunc);
		}
	}
};


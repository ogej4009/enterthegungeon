#pragma once
#include "Ref.h"
#include "Input.h"

typedef struct _tagAddFunction
{
	string	strName; // ����� �±� 
	void* pObj; // �ش� ������Ʈ  
	string	strFuncName; // ��� �Լ��̸� 

	_tagAddFunction()
	{
		pObj = nullptr;
	}
}AddFunction, * PAddFunction;

class CInputComponent :
	public CRef
{
	// ������Ʈ�� ����ϴ� Ŭ���� ��� 
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
		// ������Ʈ�� �ѹ� ���ļ� BindAxis�� ȣ���Ѵ�. 
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
		// ������Ʈ�� �ѹ� ���ļ� BindAction�� ȣ���Ѵ�.  
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


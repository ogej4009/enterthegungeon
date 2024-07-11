#include "InputComponent.h"


CInputComponent::CInputComponent()
{
}


CInputComponent::~CInputComponent()
{
	list<AddFunction>::iterator	iter;
	list<AddFunction>::iterator	iterEnd = m_FunctionAxisList.end();

	for (iter = m_FunctionAxisList.begin(); iter != iterEnd; ++iter)
	{
		// 어디서든 쉽게 쓴다. 
		GET_SINGLE(CInput)->DeleteBindAxis((*iter).strName, (*iter).pObj, (*iter).strFuncName);
	}

	iterEnd = m_FunctionActionList.end();

	for (iter = m_FunctionActionList.begin(); iter != iterEnd; ++iter)
	{
		// 어디서든 쉽게 쓴다. 
		GET_SINGLE(CInput)->DeleteBindAction((*iter).strName, (*iter).pObj, (*iter).strFuncName);
	}
}
